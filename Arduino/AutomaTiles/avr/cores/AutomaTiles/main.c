#include <AutomaTiles.h>
#include <Arduino.h>
#include "APA102C.h"

uint32_t timer;
const uint8_t black[3] = {0x00, 0x00, 0x00};
const uint8_t transmitColor[3] = {0xff, 0x55, 0x00};
const uint8_t recieveColor[3] = {0x55, 0xff, 0x00};

static uint8_t seqNum = 0;//Sequence number used to prevent circular retransmission of data

int main(void) {
	tileSetup();
	
	setup();
	
	timer = getTimer();
	while(1){
		if(mode == running){
			uint32_t diff = getTimer()-timer;
			if(holdoff<diff){
				holdoff = 0;
			}else{
				holdoff -= diff;
			}
			uint8_t t = timer%64;
			if(t<=32 && t+diff>=32){
				sendColor(LEDCLK, LEDDAT,outColor);
			}
			timer = getTimer();
			
			if(timeout>0){
				if(timer-sleepTimer>1000*timeout){
					mode = sleep;
					disAD();
					DDRB &= ~IR;//Set direction in
					PORTB &= ~IR;//Set pin tristated
					sendColor(LEDCLK, LEDDAT, black);
					PORTA |= POWER;//Set LED and Mic power pin high (off)
					wake = 0;
				}
			}
			
			loop();
		}else if(mode==recieving){
			//disable A/D
			disAD();
			//set photo transistor interrupt to only trigger on specific direction
			setDir(progDir);
			//set recieving color
			sendColor(LEDCLK, LEDDAT, recieveColor);	
			//record time entering the mode for timeout
			cli();
			uint32_t modeStart = timer;
			sei();
			while(mode==recieving){//stay in this mode until instructed to leave or timeout
				uint32_t diff = getTimer()-modeStart;
				if(diff>3000){//been in mode 1 for more than 5 seconds
					mode = transmitting;					
				}
			}
		}else if(mode==transmitting){
			//disable Phototransistor Interrupt
			setDirNone();
			//set LED to output
			DDRB |= IR;//Set direction out
			//send 5 pulses
			uint32_t startTime = getTimer();
			if(bitsRcvd>=8 && comBuf[0]!=seqNum){
				seqNum = comBuf[0];
				int i;
				for(i=0; i<5; i++){
					while(getTimer()==startTime){
						PORTB &= ~IR;
					}
					startTime = getTimer();
					while(getTimer()==startTime){
						PORTB |= IR;
					}
					startTime = getTimer();
				}
			}else{
				bitsRcvd = 0;
			}
			
			startTime = getTimer();
			sendColor(LEDCLK, LEDDAT, transmitColor);//update color while waiting			
			while(getTimer()<startTime+20);//pause for mode change
			startTime = getTimer();
			uint16_t timeDiff;
			uint16_t bitNum;
			while(bitsRcvd>0){
				timeDiff = (getTimer()-startTime)/PULSE_WIDTH;
				bitNum = timeDiff/2;
				if(timeDiff%2==0){//first half
					if(comBuf[bitNum/8]&(1<<bitNum%8)){//bit high
						PORTB &=  ~IR;
					}else{//bit low
						PORTB |=  IR;
					}
				}else{//second half
					if(comBuf[bitNum/8]&(1<<bitNum%8)){//bit high
						PORTB |=  IR;
						}else{//bit low
						PORTB &=  ~IR;
					}
				}
				if(bitNum>=bitsRcvd){
					bitsRcvd = 0;
				}				
			}
			//while(timer<startTime+2000);//pause for effect
			
			//done transmitting
			//re-enable A/D
			enAD();
			//re-enable all phototransistors
			setDirAll();
			setState(0);
			mode = running;

		}
	}
}