/* data format
  [0] total data length
  [1] data length
  [2] 'C'ommand or 'D'ata or 'S'tatus
  [3~(3+data length)] data
  [(3+data length+1)] ';'
  total data length = 4 + data length
*/

//communicate with Relay=======================================================
void relayInit(){
  for(int i=0;i<relayNo;i++){
    pinMode(relayCh[i], OUTPUT);
    digitalWrite(relayCh[i], HIGH);    //Turn off all relay.
  }
  delay(1000);
  //checkRelay();     //Check relay status at first. 
}

void relayControl(){  
  if(mngState.req==1){
    digitalWrite(relayCh[waterPump], LOW);
    if(mngState.mod==0){
      digitalWrite(relayCh[airPump], LOW);
    }else{
      digitalWrite(relayCh[solenoid], LOW);
    }
  }else if(mngState.req==0){
    for(int i=0;i<relayNo;i++){
      digitalWrite(relayCh[i], HIGH);
    }    
  }
  //relayState();
}

//communicate with Nextion=======================================================
//to Nextion--------------------------------------------------
void nxEnd(){
  Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}
void state2nx(){
  //Serial.printf("ctrl.hr_1.val=%d", 0); nxEnd();
  //Serial.printf("ctrl.min_1.val=%d", 0); nxEnd();
  //Serial.printf("ctrl.sec_1.val=%d", 0); nxEnd();
  Serial.printf("ctrl.bt1.val=%d", mngState.mod); nxEnd();
  Serial.printf("ctrl.bt0.val=%d", mngState.req); nxEnd();
  Serial.flush();
}
//from Nextion------------------------------------------------
void parseNx(){
  charLength = Serial.readBytesUntil(';', mngState.nx2mcu, 5);
  Serial.flush();
  mngState.mod = mngState.nx2mcu[3] - '0';
  mngState.req = mngState.nx2mcu[4] - '0';
  //Serial.printf("%d%d", mngState.mod, mngState.req);
  state2web();
  relayControl();
}

//communicate with Web===========================================================
//to Web--------------------------------------------------
void state2web(){
  String pubCtrl;
  StaticJsonDocument<48> doc;
  doc["MOD"] = mngState.mod;
  doc["REQ"] = mngState.req;        //LOW:on(1)상태
  serializeJson(doc, pubCtrl);      //SerializeJson(doc, Serial);
  webSocket.broadcastTXT(pubCtrl);  //webSocket.sendTXT(num, pubCtrl);
  pubCtrl = "";
  Serial.flush();
}
//from Web------------------------------------------------
void parseWeb(){
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, mngState.web2mcu);
  if (error){
    //Serial.printf("deSerializeJson() failed: %s", error.f_str());
    return;
  }
  delay(100);
  if (doc["ApPass"]) {
    const char *apPASS = doc["ApPass"];
    //Serial.println("deSerialized & input Struct in apPASS:" + String(apPASS));
    strncpy(mngConf.apPASS, apPASS, sizeof(mngConf.apPASS));
    mngConf.apPASS[sizeof(mngConf.apPASS) - 1] = '\0';
    //Serial.println("read apPASS Struct data:"+ String(mngConf.apPASS));
    writeROM(String(mngConf.apPASS), 10);
  }else if(doc["NetId"] && doc["NetPass"]) {
    const char *netID = doc["NetId"];
    const char *netIP = doc["NetIp"];
    const char *netGATEWAY = doc["NetGateWay"];
    const char *netSUBNET = doc["NetSubNet"];
    const char *netPASS = doc["NetPass"];
    //Serial.println("deSerialized & input Struct in netID:" + String(netID) + ", netIP:" + String(netIP) + " netGATEWAY:" + String(netGATEWAY) + " netSUBNET:" + String(netSUBNET) + " netPASS:" + String(netPASS));
    strncpy(mngConf.netID, netID, sizeof(mngConf.netID));
    mngConf.netID[sizeof(mngConf.netID) - 1] = '\0';
    strncpy(mngConf.netIP, netIP, sizeof(mngConf.netIP));
    mngConf.netIP[sizeof(mngConf.netIP) - 1] = '\0';
    strncpy(mngConf.netGATEWAY, netGATEWAY, sizeof(mngConf.netGATEWAY));
    mngConf.netGATEWAY[sizeof(mngConf.netGATEWAY) - 1] = '\0';
    strncpy(mngConf.netSUBNET, netSUBNET, sizeof(mngConf.netSUBNET));
    mngConf.netSUBNET[sizeof(mngConf.netSUBNET) - 1] = '\0';
    strncpy(mngConf.netPASS, netPASS, sizeof(mngConf.netPASS));
    mngConf.netPASS[sizeof(mngConf.netPASS) - 1] = '\0';
    //Serial.println("read netID Struct data:" + String(netID) + ", netIP:" +  String(mngConf.netIP) + ", netGATEWAY:" + String(mngConf.netGATEWAY) + ", netSUBNET:" + String(mngConf.netSUBNET) + ", netPASS:" + String(mngConf.netPASS));
    writeROM(String(mngConf.netID), 20);
    writeROM(String(mngConf.netIP), 50);
    writeROM(String(mngConf.netGATEWAY), 70);
    writeROM(String(mngConf.netSUBNET), 90);
    writeROM(String(mngConf.netPASS), 110);
  }
  EEPROM.end();
  ESP.restart();
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length){
  switch (type) {
    case WStype_DISCONNECTED: {
      //Serial.printf("[%u] Disconnected!\n", num);
      break;
    }
    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      //Serial.println("Connected from: " + ip.toString());
      state2web();
      break;
    }
    case WStype_TEXT:{
      mngState.web2mcu = String((char *)&payload[0]);
      if (mngState.web2mcu.startsWith("{") && mngState.web2mcu.endsWith("}")) {
        parseWeb(); //(mngState.web2mcu)
      }else if(mngState.web2mcu[2]=='C'){
        mngState.mod = mngState.web2mcu[3]-'0';
        mngState.req = mngState.web2mcu[4]-'0';
        //Serial.printf("Rcvd: %d%d", mngState.mod, mngState.req);
        state2nx();
        //state2web();
        relayControl();
      }
      break;
    }
    default:
      break;
  }
}
