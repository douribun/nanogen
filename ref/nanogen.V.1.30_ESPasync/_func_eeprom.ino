//EEPROM--------------------------------------------------------
String readROM(int len, int pos)
{
  String temp;
  for (int n = pos; n < len + pos; ++n) {
    if (char(EEPROM.read(n)) != ';') {
      if (isWhitespace(char(EEPROM.read(n)))) {
      }else{
        temp += String(char(EEPROM.read(n)));
      }
    }else{
      n = len + pos;
    }
  }
  return temp;
}

void readEEPROM(){
  readROM(12, 0).toCharArray(mngConf.mcuID, 12); delay(100); 
  readROM(12, 12).toCharArray(mngConf.mcuPASS, 12); delay(100); 
  readROM(32, 24).toCharArray(mngConf.netID, 32); delay(100); 
  readROM(20, 56).toCharArray(mngConf.netIP, 20); delay(100); 
  readROM(20, 76).toCharArray(mngConf.netGATEWAY, 20); delay(100); 
  readROM(20, 96).toCharArray(mngConf.netSUBNET, 20); delay(100); 
  readROM(20, 116).toCharArray(mngConf.netPASS, 20); delay(100); 
  //Serial.printf("READ EEPROM(ap inform): %s, %s/%d\n", mngConf.mcuID, mngConf.mcuPASS, strlen(mngConf.mcuPASS));
  //Serial.printf("READ EEPROM(sta inform): %s/%d, %s/%d, %s/%d, %s/%d, %s/%d\n", mngConf.netID, strlen(mngConf.netID), mngConf.netIP, strlen(mngConf.netIP), mngConf.netGATEWAY, strlen(mngConf.netGATEWAY), mngConf.netSUBNET, strlen(mngConf.netSUBNET), mngConf.netPASS, strlen(mngConf.netPASS));
}

void endROM(){
  EEPROM.commit();
  //EEPROM.end();
  Serial.begin(9600);
  //Serial.printf("com_star"); nxEnd(); //시리얼 통신으로 수신받는 모든 명령 실행 재시작  
  //Serial.printf("doevents"); nxEnd(); //화면 또는 수신 데이터 갱신 및 리프레시
  Serial.flush();
  //ESP.restart();
  //Serial.printf("rest"); nxEnd(); //display 초기화
  yield();  
}

void writeROM(String x, int pos){ //pos:start address
  x += ";";
  for (int n = pos; n < x.length() + pos; n++){
    EEPROM.write(n, x[n - pos]);
  }
}

void clearROM(int pos, int end){
  for (int i = pos; i < end; i++){
    EEPROM.write(i, 0);
  }
}

void clearAP(){
  clearROM(0, 24);
  endROM();
}
void clearSTA(){
  clearROM(24, 136);
  endROM();
}
