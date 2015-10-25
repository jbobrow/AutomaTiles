int dataPin = 3;
int freq = 4; //carrier freq in ms
void setup() {
  pinMode(dataPin,OUTPUT);

}
//{sequence #, color instruction, S0 Red, S0 Green, S0 Blue, S1 Red, S1 Green, S1 Blue}
byte data[] = {0x02,0x03, 0xff,0x00,0x00,0x00,0x00, 0xff, 0x04};
int dataLen = sizeof(data);
int count = 8;

void loop() {
  for(int i = 0; i< 5; i++){
    digitalWrite(dataPin,LOW);
    delayMicroseconds(1000);
    digitalWrite(dataPin,HIGH);
    delayMicroseconds(1000);
  }
  delay(10);
  for(int i = 0; i<dataLen; i++){
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
  delay(10000);
}
