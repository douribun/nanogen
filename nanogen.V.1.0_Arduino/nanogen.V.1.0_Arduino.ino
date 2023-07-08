int pins[] = {D5,D6,D7};      //esp8266 {D5,D6,D7}, {waterPump, airPump, gasPump};
int relayNo;
uint16_t delayTime=5000;              //pump air 제거 공회전 시간 -> relayControl()  

//communicate with Relay=======================================================
void relayInit(){
  relayNo = sizeof(pins);
  for(int i=0;i<relayNo;i++){
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], HIGH);    //Turn off all relay.
  }  
  delay(1000);
}

void LedTimer(){
  for(int i=0;i<delayTime/1000;i++){
    digitalWrite(BUILTIN_LED, LOW);
    delay(500);
    digitalWrite(BUILTIN_LED, HIGH);
    delay(500);
  }  
}

void relayControl(byte _mod, byte _req){
  //Serial.print(_mod); Serial.print(_req);   //for check
  if(_req==1){
    digitalWrite(pins[0], LOW);
    LedTimer();
    if(_mod==0){
      digitalWrite(pins[1], LOW);
    }else{
      digitalWrite(pins[2], LOW);
    }
    digitalWrite(BUILTIN_LED, LOW);
  }else if(_req==0){
    for(int i=1;i<relayNo;i++){
      digitalWrite(pins[i], HIGH);
    }
    LedTimer();
    digitalWrite(pins[0], HIGH);
    digitalWrite(BUILTIN_LED, HIGH);
  }
}

void parseNx(){
  char buffer[10]={0,};
  int charLength = Serial.readBytesUntil(';', buffer, 10);
  Serial.flush();
  if(buffer[2] == 'C'){
    byte mod = buffer[3] - '0';
    byte req = buffer[4] - '0';
    //Serial.print(mod); Serial.print(req);     //for check
    relayControl(mod, req);
  }
  //Serial.flush();
}

void setup(){
  Serial.begin(9600);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);  
  relayInit();                            //delay time
}

void loop(){
  if (Serial.available()) {
    //Serial.printf("rcv:%s\n", buffer);
    parseNx();
  }
}