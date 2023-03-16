//CTRLER-----------------------------------------------------------------------
String CTRL_THINGS(){
  String THINGS_HTML = FPSTR(INDEX_HEAD);
  THINGS_HTML += "<h3 align: center>";
  THINGS_HTML += mngConf.mcuID;
  THINGS_HTML += "</h3>";
  THINGS_HTML += FPSTR(BODY_CTRL);
  if(sizeof mngState.errorMsg > 20){
    THINGS_HTML += mngState.errorMsg;
    Serial.printf(mngState.errorMsg); nxEnd();
  }
  THINGS_HTML += FPSTR(BODY_CONFIG1);
  int No_SSID = WiFi.scanNetworks();
  if (No_SSID == -1){
    THINGS_HTML += "None Network!";
    while (true);
  }else{
    for (int i = 0; i<No_SSID; i++) {
      THINGS_HTML += "<option value='";
      THINGS_HTML += WiFi.SSID(i);
      THINGS_HTML += "'>";
      THINGS_HTML += WiFi.SSID(i);
      THINGS_HTML += "</option>";
    }
  }
  THINGS_HTML += FPSTR(BODY_CONFIG2);
  return THINGS_HTML;
}

//handler-------------------------------------------------------
bool loadFromSpiffs(String path){
  String dataType = "text/plain";
  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".png")) dataType = "image/png";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (webServer.hasArg("download")) dataType = "application/octet-stream";
  if (webServer.streamFile(dataFile, dataType) != dataFile.size()) {
  }
  dataFile.close();
  return true;
}

void handleNotFound(){
  if(loadFromSpiffs(webServer.uri())) return;
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += webServer.uri();
  message += "\nMethod: ";
  message += (webServer.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += webServer.args();
  message += "\n";
  for (uint8_t i = 0; i < webServer.args(); i++)
  {
    message += " " + webServer.argName(i) + ": " + webServer.arg(i) + "\n";
  }
  webServer.send(404, "text/plain", message);
  //Serial.println(message);
}

void handleRoot(){
  webServer.send(200, "text/html; charset=UTF-8", CTRL_THINGS());
}
