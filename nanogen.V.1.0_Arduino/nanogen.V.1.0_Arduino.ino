//const byte waterPump=D5;const byte airPump=D6;const byte gasPump=D7;
const byte relayPin[] = {D5, D6, D7, D8};
byte len = sizeof(relayPin);
const byte airOn[] = {LOW, LOW, HIGH, HIGH};   //LOW=ON
const byte gasOn[] = {LOW, HIGH, LOW, HIGH};

class Relay{
private:
  byte pin;
  byte turn;

public:
  Relay(){
    init();
  }
  void init(){
    for(int i=0;i<len;i++){
      pinMode(relayPin[i], OUTPUT);
      act(relayPin[i], HIGH);
    }
    pinMode(BUILTIN_LED, OUTPUT);    
  }

  byte state(byte _pin){
    pin = _pin;
    return digitalRead(pin);
  }

  void act(byte _pin, byte _turn){
    pin = _pin;
    turn = _turn;
    if (state(pin) != turn){
      digitalWrite(pin, turn);
      digitalWrite(BUILTIN_LED, turn);
    }
  }
};

class NexCmd{
  //int charLength;
  char buffer[10]={0,}; 
public:
  int charLength = Serial.readBytesUntil(';', buffer, 10);   
  byte parseNx(){    
    Serial.flush();
    if(buffer[2] == 'C'){
      return buffer[3] - '0', buffer[4] - '0';
    }else return 0;
  }
};

void control(Relay _rel, byte _mod, byte _req){  
  if(_req == 1){
    if(_mod == 0){
      for(int i=0;i<len;i++){
        _rel.act(relayPin[i], airOn[i]);
      }
    }else if(_mod == 1){
      for(int i=0;i<len;i++){
        _rel.act(relayPin[i], gasOn[i]);
      }
    }
  }else{
    for(int i=0;i<len;i++){
      _rel.act(relayPin[i], HIGH);
    }
  }
}

NexCmd nex1;
Relay rel1;

void setup(){  //=============================================
  Serial.begin(115200);  
  rel1.init();
}

void loop(){//==============================================
  if (Serial.available()) {      
    byte mo, re = nex1.parseNx();
    Serial.printf("Nextion data: mod=%d, req=%d\n", mo, re);
    control(rel1, mo, re);
  }
}