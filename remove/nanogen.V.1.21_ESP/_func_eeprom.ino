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
  readROM(12, 0).toCharArray(mngConf.mcuID, 12); delay(100);  //sizeof()->?��릿수 10, strlen()->9 or 0  
  readROM(12, 12).toCharArray(mngConf.apPASS, 12); delay(100); 
  readROM(32, 24).toCharArray(mngConf.netID, 32); delay(100); 
  readROM(20, 56).toCharArray(mngConf.netIP, 20); delay(100); 
  readROM(20, 76).toCharArray(mngConf.netGATEWAY, 20); delay(100); 
  readROM(20, 96).toCharArray(mngConf.netSUBNET, 20); delay(100); 
  readROM(20, 116).toCharArray(mngConf.netPASS, 20); delay(100); 
  //Serial.printf("READ EEPROM(ap inform): %s, %s/%d\n", mngConf.mcuID, mngConf.apPASS, strlen(mngConf.apPASS));
  //Serial.printf("READ EEPROM(sta inform): %s/%d, %s/%d, %s/%d, %s/%d, %s/%d\n", mngConf.netID, strlen(mngConf.netID), mngConf.netIP, strlen(mngConf.netIP), mngConf.netGATEWAY, strlen(mngConf.netGATEWAY), mngConf.netSUBNET, strlen(mngConf.netSUBNET), mngConf.netPASS, strlen(mngConf.netPASS));
}

void clearROM(int pos, int end){
  for (int i = pos; i < end; i++){
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
  readEEPROM();
}

void writeROM(String x, int pos){ //pos:start address
  x += ";";
  for (int n = pos; n < x.length() + pos; n++){
    EEPROM.write(n, x[n - pos]);
  }
  //Serial.println("write to EEPROM: " + x);
  EEPROM.commit();
}

void clearAP(){clearROM(0, 24); ESP.restart();}
void clearSTA(){clearROM(24, 136); ESP.restart();}