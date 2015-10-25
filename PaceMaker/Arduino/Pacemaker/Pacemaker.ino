void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  int time = millis()%500;
  if(time==1||time==3||time==5||time==7){
    digitalWrite(3, HIGH);
  }else{
    digitalWrite(3, LOW);
  }

  digitalWrite(13,digitalRead(2));  
}
