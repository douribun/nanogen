void handleNotFound(AsyncWebServerRequest *request){
  /*
  Serial.printf("NOT_FOUND: ");
  if(request->method() == HTTP_GET)
    Serial.printf("GET");
  else if(request->method() == HTTP_POST)
    Serial.printf("POST");
  else if(request->method() == HTTP_DELETE)
    Serial.printf("DELETE");
  else if(request->method() == HTTP_PUT)
    Serial.printf("PUT");
  else if(request->method() == HTTP_PATCH)
    Serial.printf("PATCH");
  else if(request->method() == HTTP_HEAD)
    Serial.printf("HEAD");
  else if(request->method() == HTTP_OPTIONS)
    Serial.printf("OPTIONS");
  else
    Serial.printf("UNKNOWN");
  Serial.printf(" http://%s%s\n", request->host().c_str(), request->url().c_str());

  if(request->contentLength()){
    Serial.printf("_CONTENT_TYPE: %s\n", request->contentType().c_str());
    Serial.printf("_CONTENT_LENGTH: %u\n", request->contentLength());
  }
  */
  request->send(404);
}

String mcuID_pl(const String &var){
  if(var == "MCUID"){
    return String(mngConf.mcuID);
  }
  return String();
}

void handleRoot(AsyncWebServerRequest *request){
  request->send(SPIFFS, "/index.html", "text/html", false, mcuID_pl);
}

void initWebServer(){
  //Configuring Web Server-------------------------------------
  webServer.on("/", handleRoot);
  webServer.onNotFound(handleNotFound);
  webSocket.onEvent(webSocketEvent);
  webServer.addHandler(&webSocket);  
  webServer.serveStatic("/", SPIFFS, "/");
  webServer.begin();
}
