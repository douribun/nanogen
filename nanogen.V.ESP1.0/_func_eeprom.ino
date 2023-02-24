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
  readROM(9, 0).toCharArray(mngConf.mcuID, 10);  //sizeof()->자릿수 10, strlen()->9 or 0  
  readROM(10, 10).toCharArray(mngConf.apPASS, 10);
  readROM(30, 20).toCharArray(mngConf.netID, 30);
  readROM(20, 50).toCharArray(mngConf.netIP, 20);
  readROM(20, 70).toCharArray(mngConf.netGATEWAY, 20);
  readROM(20, 90).toCharArray(mngConf.netSUBNET, 20);
  readROM(20, 110).toCharArray(mngConf.netPASS, 20);
  delay(100);   
  //Serial.printf("READ EEPROM(ap inform): %s/%d\n", mngConf.apPASS, strlen(mngConf.apPASS));
  //Serial.printf("READ EEPROM(sta inform): %s/%d, %s/%d, %s/%d, %s/%d, %s/%d\n", mngConf.netID, strlen(mngConf.netID), mngConf.netIP, strlen(mngConf.netIP), mngConf.netGATEWAY, strlen(mngConf.netGATEWAY), mngConf.netSUBNET, strlen(mngConf.netSUBNET), mngConf.netPASS, strlen(mngConf.netPASS));
}

void blinking(){
  for (int i = 0; i < 5; i++){
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
  }
}

void clearROM(int pos, int end){
  for (int i = pos; i < end; i++){
    EEPROM.write(i, 0);
  }
  blinking();
  EEPROM.commit();
  readEEPROM();
}

void writeROM(String x, int pos){
  x += ";";
  for (int n = pos; n < x.length() + pos; n++){
    EEPROM.write(n, x[n - pos]);
  }
  //Serial.println("write to EEPROM: " + x);
  EEPROM.commit();
}

void clearROM_NET(){
  //Serial.printf("IoT용도와 네트워크정보를 삭제합니다.\n");
  clearROM(20, 130);    //60:except "mcuID/apPASS"
  ESP.restart();
}
