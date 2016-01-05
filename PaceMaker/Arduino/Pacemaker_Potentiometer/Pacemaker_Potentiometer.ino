// Pacemaker w/ Potentiometer
int txPin = 3;
int rxPin = 4;
int potPin = 0;

float fps = 6;

void setup() {
  pinMode(txPin,OUTPUT);
  pinMode(rxPin,INPUT);
  Serial.begin(9600);
}

void loop() {
  int potVal = analogRead(potPin);
  
  fps = 5.0 * ((1024.0 - potVal) / 1024.0) + 1.0;
  Serial.println(fps); 

  
  // send 3 pulses
  for(int i = 0; i< 3; i++){
    digitalWrite(txPin,HIGH);
    delayMicroseconds(1000);
    digitalWrite(txPin,LOW);
    delayMicroseconds(1000);
  }
  
  // wait til next frame
  delay(1000.0/fps);
}
