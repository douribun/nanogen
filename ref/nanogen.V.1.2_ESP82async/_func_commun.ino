/* data format
  [0] total data length
  [1] data length
  [2] 'C'ommand or 'D'ata or 'S'tatus
  [3~(3+data length)] data
  [(3+data length+1)] ';'
  total data length = 4 + data length
*/

//communicate with Relay=======================================================
void initRelay(){
  for(int i=0;i<relayNo;i++){
    pinMode(relayCh[i], OUTPUT);
    digitalWrite(relayCh[i], HIGH);    //Turn off all relay.
  }
  delay(1000);
}

void relayControl(){  
  if(mngState.req==1){
    digitalWrite(relayCh[0], LOW);
    if(mngState.mod==0){
      digitalWrite(relayCh[1], LOW);
    }else{
      digitalWrite(relayCh[2], LOW);
    }
  }else if(mngState.req==0){
    for(int i=0;i<relayNo;i++){
      digitalWrite(relayCh[i], HIGH);
    }    
  }
}

//communicate with Nextion=======================================================
//to Nextion--------------------------------------------------
void nxEnd(){
  Serial.write(0xff); Serial.write(0xff); Serial.write(0xff);
}
void state2nx(){
  Serial.printf("ctrl.bt1.val=%d", mngState.mod); nxEnd();
  Serial.printf("ctrl.bt0.val=%d", mngState.req); nxEnd();
  Serial.flush();
}
//from Nextion------------------------------------------------
void parseNx(){
char nx2mcu[6]={0,};
  charLength = Serial.readBytesUntil(';', nx2mcu, 6);
  delay(50);
  if(nx2mcu[2] == 'C'){
    mngState.mod = nx2mcu[3] - '0';
    mngState.req = nx2mcu[4] - '0';
  }
  delay(100);
  Serial.flush();
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
  webSocket.textAll(pubCtrl);       //webSocket.broadcastTXT(pubCtrl);  //webSocket.sendTXT(num, pubCtrl);
  pubCtrl = "";
  Serial.flush();
}

void scan2web(){
  if(wifiJson.length() > 14){  //131
    //Serial.printf("the wifiJson to web: (%d) %s\n", wifiJson.length(), wifiJson.c_str());
    webSocket.textAll(wifiJson);
  }else{
    WiFi.scanNetworksAsync(prinScanResult);
  }  
}

//from Web------------------------------------------------
void parseWeb(String web2mcu){
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, web2mcu);
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
    writeROM(String(mngConf.apPASS), 12);
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
    writeROM(String(mngConf.netID), 24);
    writeROM(String(mngConf.netIP), 56);
    writeROM(String(mngConf.netGATEWAY), 76);
    writeROM(String(mngConf.netSUBNET), 96);
    writeROM(String(mngConf.netPASS), 116);
  }
  EEPROM.commit();
  yield();
  //EEPROM.end();
  //ESP.restart();
}

void handleWSMessage(void *arg, uint8_t *data, size_t len){
  AwsFrameInfo *info = (AwsFrameInfo*)arg;      
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;    
    String web2mcu = (char *)&data[0];
    //Serial.printf("recieved from Web: %s\n", web2mcu);
    if(web2mcu.startsWith("W")){
      scan2web();
    }else if (web2mcu.startsWith("{") && web2mcu.endsWith("}")){
      parseWeb(web2mcu);
    }else if(web2mcu.startsWith("5")){ 
      mngState.mod = web2mcu[3]-'0';
      mngState.req = web2mcu[4]-'0';
      //Serial.printf("Rcvd: %d%d\n", mngState.mod, mngState.req);
      state2nx();
      state2web();  //for web to web
      relayControl();
    }else if(web2mcu.startsWith("a")){
      clearAP();
    }else if(web2mcu.startsWith("s")){
      clearSTA();
    }
  }
}

void webSocketEvent(AsyncWebSocket       *server,
                    AsyncWebSocketClient *client,
                    AwsEventType          type,
                    void                 *arg,
                    uint8_t              *data,
                    size_t                len) {
  switch (type) {
    case WS_EVT_DISCONNECT:
      //Serial.printf("WebSocket client #%u disconnected\n", client->id());
      wsClient = nullptr;
      break;
    case WS_EVT_CONNECT:
      //Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      wsClient = client;
      state2web();
      break;
    case WS_EVT_DATA:
      handleWSMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}
