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
    IP.fromString(String(mngConf.netIP));
    GATEWAY.fromString(String(mngConf.netGATEWAY));
    SUBNET.fromString(String(mngConf.netSUBNET));
    //Serial.printf("IP: %s, GATEWAY: %s, SUBNET: %s\n", String(IP[3]), String(GATEWAY[3]), String(SUBNET[3]));
    WiFi.config(IP, GATEWAY, SUBNET);
    WiFi.begin(mngConf.netID, mngConf.netPASS);    
    int count = 0;
    while ((WiFi.status() != WL_CONNECTED) && count < 15) {
      delay(500);
      count++;
    }
    //Serial.printf("Connecting WiFi... netID: %s, netPASS: %s, netIP: %s, netGateWay: %s, netSubNet: %s\n", mngConf.netID, mngConf.netPASS, mngConf.netIP, mngConf.netGATEWAY, mngConf.netSUBNET);
  }
  if (WiFi.status() != WL_CONNECTED){
    //Serial.printf("Missing or invalid network information, Try to connect to AP mode.\n");
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
  if(strlen(mngConf.mcuID)!= 9 || mngConf.mcuID[0]!='I' || mngConf.mcuID[3]!='M' || strlen(mngConf.apPASS) < 8){
    makemcuID();
  }
  //Serial.printf("mcuID lD: %s, %d\n", mngConf.mcuID, strlen(mngConf.mcuID)); 
    
  connectSTA();
  delay(10);
  
  //Configuring Web Server-------------------------------------
  webServer.setContentLength(CONTENT_LENGTH_UNKNOWN);
  webServer.on("/", handleRoot);
  webServer.on("/apClear", clearAP);
  webServer.on("/staClear", clearSTA);
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
