void jsonNetworks(int netNo){
  StaticJsonDocument<256> doc;
  JsonArray SSID = doc.createNestedArray("SSID");
  for (int i = 0; i < netNo; i++){
    SSID.add(WiFi.SSID(i));
  }
  serializeJson(doc, wifiJson);
  delay(500);
  Serial.printf("(%d) %s\n", netNo, wifiJson.c_str());
  //{"SSID":["bong-gu","[floor a/c]_E30AJT0003474M","U+Net71A3","SK_WiFiGIGA5D70_2.4G","kims","kyy2","sull","SK_WiFi46B6"]}
}

#ifdef ESP8266
  void prinScanResult(int networksFound){
    if (n == 0) {
      //Serial.println("no networks found");
    } else if(networksFound > 0){
      jsonNetworks(networksFound);
      
    }
  }
#else
  void scanNet_Esp32(){
    Serial.print("Scan start ... "); 
    int n = WiFi.scanNetworks();
    if (n == 0) {
      //Serial.println("no networks found");
    } else {
      jsonNetworks(n);
    }
  }
#endif

void scanWorks(){
  #ifdef ESP8266
    WiFi.scanNetworksAsync(prinScanResult);
  #else
    scanNet_Esp32();
  #endif
}

void connectAP(){
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  //scanWorks();
  delay(100);
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
  EEPROM.begin(136); 
  Serial.begin(115200);
  
  //start------------------------------------------------------   
  if(!SPIFFS.begin()){while(1);}
  readEEPROM();
  if(strlen(mngConf.mcuID)!= 9 || mngConf.mcuID[0]!='I' || mngConf.mcuID[3]!='M' || strlen(mngConf.apPASS) < 8){
    makemcuID();
  }
  //Serial.printf("mcuID lD: %s, %d\n", mngConf.mcuID, strlen(mngConf.mcuID)); 
  initRelay();  
  connectSTA();
  scanWorks();
  delay(10);
    
  //Serial.printf("Webserver & WebSocket connected: ws://%s, %s:/ws\n", WiFi.softAPIP().toString(), WiFi.localIP().toString());  
  initWebServer();
}

void loop(){//==============================================
  webSocket.cleanupClients();
  if (Serial.available()) {
    parseNx();
  }
}
