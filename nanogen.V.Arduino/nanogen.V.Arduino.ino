const byte relayNo=3;   //3,const byte waterPump=0;const byte airPump=1;const byte gasPump=2;
const byte THINGS[relayNo]={D5, D6, D7};      //esp8266 {D5,D6,D7}
//const uint16_t delayTime=5000;              //pump air 제거 공회전 시간 -> relayControl()                  

typedef struct _CmdData{
  char buffer[10];
  char errorMsg[25];
  int charLength=0;  
  byte mod=0;
  byte req=0;
} CmdData;
CmdData mngCmd;

//communicate with Relay=======================================================
void relayInit(){
  for(int i=0;i<relayNo;i++){
    pinMode(THINGS[i], OUTPUT);
    digitalWrite(THINGS[i], HIGH);    //Turn off all relay.
  }
  delay(1000);
}

void relayControl(){
  //Serial.print(mngCmd.mod); Serial.print(mngCmd.req);   //for check
  if(mngCmd.req==1){
    digitalWrite(THINGS[0], LOW);
    //delay(delayTime);
    if(mngCmd.mod==0){
      //digitalWrite(THINGS[1], LOW);
    }else{
      digitalWrite(THINGS[2], LOW);
    }
    digitalWrite(BUILTIN_LED, LOW);    
  }else if(mngCmd.req==0){
    for(int i=0;i<relayNo;i++){
      digitalWrite(THINGS[i], HIGH);
    }
    digitalWrite(BUILTIN_LED, HIGH);    
  }  
}

//from Nextion------------------------------------------------
void parseNx(){
  mngCmd.buffer[10]={0,};
  mngCmd.charLength = Serial.readBytesUntil(';', mngCmd.buffer, 10);
  //Serial.flush();
  if(mngCmd.buffer[2] == 'C'){
    mngCmd.mod = mngCmd.buffer[3] - '0';
    mngCmd.req = mngCmd.buffer[4] - '0';
    //Serial.print(mngCmd.mod); Serial.print(mngCmd.req);     //for check
    relayControl();
  }
  Serial.flush();
}

void setup(){  //=============================================
  Serial.begin(9600);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  relayInit();
}

void loop(){//==============================================
  if (Serial.available()) {   //nxSerial
    //Serial.printf("rcv:%s", mngCmd.buffer);
    parseNx();
  }
}
