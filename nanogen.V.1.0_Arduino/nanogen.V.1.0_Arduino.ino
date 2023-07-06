//const byte waterPump=D5;const byte airPump=D6;const byte gasPump=D7;
#define RelayPin {D5, D6, D7, D8}
#define airOn {"LOW", "LOW", "HIGH", "HIGH"}   //LOW=ON
#define gasOn {"LOW", "HIGH", "LOW", "HIGH"}

typedef struct _CmdData{
  char buffer[10]={0,};
  char errorMsg[25];
  int charLength=0;  
  byte mod=0;
  byte req=0;
} CmdData;
CmdData getCmd;

//communicate with Relay=======================================================
class Relay{
private:
  byte _pin;
  byte _state;

public:
  Relay(byte _pin){
    this._pin = _pin;
    init();
  }

  void init(){        //void relayInit(){
    pinMode(_pin, OUTPUT);
    off();    //Turn off all relay.
  }
}

//from Nextion------------------------------------------------
class NexCmd{
private:
  /* data */
public:
  void parseNx(){
    getCmd.charLength = Serial.readBytesUntil(';', getCmd.buffer, 10);
    //Serial.flush();
    if(getCmd.buffer[2] == 'C'){
      getCmd.mod = getCmd.buffer[3] - '0';
      getCmd.req = getCmd.buffer[4] - '0';
      //Serial.print(getCmd.mod); Serial.print(getCmd.req);     //for check
      control(getCmd.mod, getCmd.req);
    }
    Serial.flush();
  }
  void control(byte _mod, byte _req){
    if(_req==1){      
        for(int i=0;i<RelayPinNo;i++){
          if(_mod==0){
            digitalWrite(RelayPin[i], airOn[i]);
          }else if(_mod==1){
            digitalWrite(RelayPin[i], gasOn[i]);
          }
        }
        digitalWrite(BUILTIN_LED, LOW);  
    }else{
      for(int i=0;i<RelayPinNo;i++){
        digitalWrite(RelayPin[i], HIGH);
      }
      digitalWrite(BUILTIN_LED, HIGH);    
    }
  }
};



void setup(){  //=============================================
  Serial.begin(9600);
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
  Relay.init();
}

void loop(){//==============================================
  if (Serial.available()) {   //nxSerial
    //Serial.printf("rcv:%s", getCmd.buffer);
    NexCmd.parseNx();
  }
}