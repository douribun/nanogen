void connectAP(){
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  delay(10);
  WiFi.softAP(mngConf.mcuID, mngConf.apPASS);
  //Serial.printf("스마트폰 'WiFi/사용 가능한 네트워크'에서 %s를 비번 %s로 연결하세요!\n", mngConf.mcuID, mngConf.apPASS);
}

void connectSTA(){
  IPAddress IP;
  IPAddress GATEWAY;
  IPAddress SUBNET;
  if (String(mngConf.netID).length() > 0 && String(mngConf.netPASS).length() > 0){
    WiFi.mode(WIFI_STA);
    Serial.printf("Connecting WiFi... netIP: %s, netGateWay: %s, netSubNet: %s", mngConf.netIP, mngConf.netGATEWAY, mngConf.netSUBNET);
    IP.fromString(String(mngConf.netIP));
    GATEWAY.fromString(String(mngConf.netGATEWAY));
    SUBNET.fromString(String(mngConf.netSUBNET));
    //Serial.printf("IP: %s, GATEWAY: %s, SUBNET: %s", String(IP), String(GATEWAY), String(SUBNET));

    WiFi.config(IP, GATEWAY, SUBNET);
    WiFi.begin(mngConf.netID, mngConf.netPASS);
    Serial.printf("Connecting WiFi..., read netID: %s, netPASS: %s", mngConf.netID, mngConf.netPASS);  
    int count = 0;
    while ((WiFi.status() != WL_CONNECTED) && count < 20) {
      delay(500);
      count++;
    }
  }
  if (WiFi.status() != WL_CONNECTED){
    connectAP();
  }
}

void setup(){  //=============================================
  EEPROM.begin(130); 
  Serial.begin(115200);
  handleFile();
  relayInit();

  //start------------------------------------------------------    
  readEEPROM();
  if(strlen(mngConf.mcuID)!= 9 || mngConf.mcuID[0]!='I'){
    makemcuID();
  }
  //Serial.printf("mcuID lD: %d\n", strlen(mngConf.mcuID)); 
    
  connectSTA();
  delay(10);
  
  //Configuring Web Server-------------------------------------
  webServer.on("/", handleRoot);
  webServer.on("/netClear", clearROM_NET);
  webServer.onNotFound(handleNotFound);

  webServer.begin();
  //Serial.printf("Webserver & WebSocket connected: ws://%s, %s:81/\n", WiFi.softAPIP().toString(), WiFi.localIP().toString());
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop(){//==============================================
  webServer.handleClient();
  webSocket.loop();  
  if (Serial.available()) {   //nxSerial
    parseNx();
  }
}
