int dataPin = 3;
int freq = 4; //carrier freq in ms
void setup() {
  pinMode(dataPin,OUTPUT);
  Serial.begin(9600);
}
//{sequence #, color instruction, S0 Red, S0 Green, S0 Blue, S1 Red, S1 Green, S1 Blue}
byte colorData[] = {0x02,0x03, 0xff,0x00,0x00,0x00,0x00, 0xff};
byte ruleData[] = {0x02,0x01,0b0001100, 0b0000110};
byte soundOnData[] = {0x02,0x05};
byte soundOffData[] = {0x02,0x04};
int colorDataLen = sizeof(colorData);
int ruleDataLen = sizeof(ruleData);
int soundOnLen = sizeof(soundOnData);
int soundOffLen = sizeof(soundOffData);
int count = 8;

int resp;
void loop() {
  Serial.write("\n\rEnter Selection:\n\r1: Change Color\n\r2: Send Last Color\n\r");
  Serial.write("3: Turn On Sound\n\r4: Turn Off Sound\n\r5: Start Pacemaker\n\r")
  resp = readInt();
  if(resp<1 || resp>5){
    Serial.write("\n\rInvalid Input\n\r");
  }else{
    if(resp==1){
      getColors();
      sendData(colorData, colorDataLen);
      Serial.write("\n\rColors Updated\n\r");
    }else if(resp==2){
      sendData(colorData, colorDataLen);
      Serial.write("\n\rColors Updated\n\r");
    }else if(resp==3){
      sendData(soundOnData,soundOnLen);
      Serial.write("\n\rMicrophone Enabled\n\r");
    }else if(resp==4){
      sendData(soundOffData,soundOffLen);
      Serial.write("\n\rMicrophone Disabled\n\r");
    }else if(resp==5){
      Serial.write("\n\rSend Any Character To Stop\n\r");
      while(Serial.available() == 0){
        for(int i = 0; i< 4; i++){
          digitalWrite(dataPin,HIGH);
          delayMicroseconds(1000);
          digitalWrite(dataPin,LOW);
          delayMicroseconds(1000);
        }
        delay(500);
      }
      readInt();//clear out sent message
    }
  }  
}

byte runoutBuf[10];
int readInt(){
  while(Serial.available() == 0);
  int res = Serial.parseInt();
  Serial.readBytes(runoutBuf,10);
  return res;
}

void getColors(){
  Serial.write("\n\rEnter Off Red Value (0-255)\n\r");
  colorData[2] = fixVal(readInt());
  Serial.write("\n\rEnter Off Green Value (0-255)\n\r");
  colorData[3] = fixVal(readInt());
  Serial.write("\n\rEnter Off Blue Value (0-255)\n\r");
  colorData[4] = fixVal(readInt());
  Serial.write("\n\rEnter On Red Value (0-255)\n\r");
  colorData[5] = fixVal(readInt());
  Serial.write("\n\rEnter On Green Value (0-255)\n\r");
  colorData[6] = fixVal(readInt());
  Serial.write("\n\rEnter On Blue Value (0-255)\n\r");
  colorData[7] = fixVal(readInt());  
}

int fixVal(int a){
  if(a<0)return 0;
  if(a>255)return 255;
  return a;
}

void sendData(byte* data, unsigned int len){
  data[0] = count;
  for(int i = 0; i< 5; i++){
    digitalWrite(dataPin,LOW);
    delayMicroseconds(1000);
    digitalWrite(dataPin,HIGH);
    delayMicroseconds(1000);
  }
  delay(10);
  for(int i = 0; i<len; i++){
    for(int j = 0; j<8; j++){
      int b = bitRead(data[i],j);//LSB first
      digitalWrite(dataPin,!b);//0 = High -> Low; 1 = Low -> High; IEEE 802.3 convention
      delay(freq);
      digitalWrite(dataPin,b);
      delay(freq);
    }
  }
  count += 2;
  if(count > 32){//changing number to assist with data monitoring 
    count = 0;
  }
  data[0] = count;
}
