#include <Arduino.h>
#line 1 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\nanogen.V.ESP.ino"
/*
  author : jus.kim
  date : 2022.11.11
*/

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266mDNS.h>
  #include <FS.h>
  #include <Servo.h>
#else
  #include <WiFi.h>
  #include <WebServer.h>
  #include <ESPmDNS.h>
  #include <SPIFFS.h>
  #include <ESP32_Servo.h>
#endif
#include <WebSocketsServer.h>
#include <EEPROM.h>
#include <ArduinoJson.h>
#include "index.h"

#ifdef ESP8266
  ESP8266WebServer webServer(80);
#else
  WebServer webServer(80);
#endif
WebSocketsServer webSocket = WebSocketsServer(81);

IPAddress IP;
IPAddress GATEWAY;
IPAddress SUBNET;

const int relayNo=3;
#ifdef ESP8266
  const byte relayCh[relayNo]    = {D5, D6, D7};    //{D5, D6, D8}/D7 no, before{D2, D5, D6} //되도록 0(D3),2(D4),15(D8)은 사용하지 마라)-------------------------
#else
  const byte LED_BUILTIN         = 2;
  const byte relayCh[relayNo]    = {21, 19, 18};    //{21, 19, 18} before{25, 26, 27}
#endif
const int waterPump=0;
const int airPump=1;
const int solenoid=2;

int charLength=0;
typedef struct _StateData{
  int mod=0;
  int req=0;
  char nx2mcu[5];
  String web2mcu;
  char errorMsg[25];
} StateData;
StateData mngState;

typedef struct _ConfigData{
  char mcuID[10];
  char apPASS[10];
  char netID[30];
  char netIP[20];
  char netGATEWAY[20];
  char netSUBNET[20];
  char netPASS[20];
} ConfigData;
ConfigData mngConf;

#line 67 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\nanogen.V.ESP.ino"
void makemcuID();
#line 11 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_commun.ino"
void relayInit();
#line 20 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_commun.ino"
void relayControl();
#line 38 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_commun.ino"
void nxEnd();
#line 41 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_commun.ino"
void state2nx();
#line 50 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_commun.ino"
void parseNx();
#line 62 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_commun.ino"
void state2web();
#line 73 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_commun.ino"
void parseWeb();
#line 116 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_commun.ino"
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
#line 2 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_eeprom.ino"
String readROM(int len, int pos);
#line 18 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_eeprom.ino"
void readEEPROM();
#line 31 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_eeprom.ino"
void blinking();
#line 40 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_eeprom.ino"
void clearROM(int pos, int end);
#line 49 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_eeprom.ino"
void writeROM(String x, int pos);
#line 58 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_eeprom.ino"
void clearROM_NET();
#line 2 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_webserver.ino"
String CTRL_THINGS();
#line 31 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_webserver.ino"
bool loadFromSpiffs(String path);
#line 43 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_webserver.ino"
void handleNotFound();
#line 61 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_webserver.ino"
void handleFile();
#line 66 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_webserver.ino"
void handleRoot();
#line 1 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\main.ino"
void connectAP();
#line 9 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\main.ino"
void connectSTA();
#line 32 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\main.ino"
void setup();
#line 67 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\main.ino"
void loop();
#line 67 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\nanogen.V.ESP.ino"
void makemcuID(){
  uint8_t MAC_array[6];
  WiFi.macAddress(MAC_array);  
  sprintf(mngConf.mcuID,"IEUM-%x%x",MAC_array[4],MAC_array[5]);
  mngConf.mcuID[sizeof(mngConf.mcuID) - 1] = '\0';
  //Serial.printf("makemcuID mcuID: %s\n", mngConf.mcuID);
  sprintf(mngConf.apPASS,"%s", "00000000"); //mcu 최초 비번(00000000)
  mngConf.apPASS[sizeof(mngConf.apPASS) - 1] = '\0';
  //Serial.printf("makemcuID password: %s\n",mngConf.apPASS);
  writeROM(String(mngConf.mcuID), 0);
  writeROM(String(mngConf.apPASS), 10);
  EEPROM.commit();
  yield();
}

#line 1 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_commun.ino"
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

#line 1 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_eeprom.ino"
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

#line 1 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\_func_webserver.ino"
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

void handleFile(){
  SPIFFS.begin();
  webServer.onNotFound(handleNotFound);
}

void handleRoot(){
  webServer.send(200, "text/html; charset=UTF-8", CTRL_THINGS());
}

#line 1 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\main.ino"
void connectAP(){
  WiFi.disconnect();
  //WiFi.mode(WIFI_AP);
  delay(10);
  WiFi.softAP(mngConf.mcuID, mngConf.apPASS);
  //Serial.printf("스마트폰 'WiFi/사용 가능한 네트워크'에서 %s를 비번 %s로 연결하세요!\n", mngConf.mcuID, mngConf.apPASS);
}

void connectSTA(){
  if (String(mngConf.netID).length() > 0 && String(mngConf.netPASS).length() > 0){
    //WiFi.mode(WIFI_STA);
    //Serial.printf("Connecting WiFi... netIP: %s, netGateWay: %s, netSubNet: %s", mngConf.netIP, mngConf.netGATEWAY, mngConf.netSUBNET);
    IP.fromString(String(mngConf.netIP));
    GATEWAY.fromString(String(mngConf.netGATEWAY));
    SUBNET.fromString(String(mngConf.netSUBNET));
    //Serial.print("IP: "+ IpAddressToString(IP)); //Serial.print(", GATEWAY" + IpAddressToString(GATEWAY)); //Serial.println(", SUBNET" + IpAddressToString(SUBNET));

    WiFi.config(IP, GATEWAY, SUBNET);
    WiFi.begin(mngConf.netID, mngConf.netPASS);
    //Serial.printf("Connecting WiFi..., read netID: %s, netPASS: %s", mngConf.netID, mngConf.netPASS);  
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
  Serial.begin(9600);
  handleFile();

  pinMode(LED_BUILTIN, OUTPUT);
  #ifdef ESP8266
    digitalWrite(LED_BUILTIN, HIGH);
  #else
    digitalWrite(LED_BUILTIN, LOW);
  #endif  

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

