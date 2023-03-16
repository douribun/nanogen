/*
  author : jus.kim
  date : 2022.11.11
*/
#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <FS.h>
#else
  #include <WiFi.h>
  #include <ESPmDNS.h>
  #include <SPIFFS.h>
#endif
#include <EEPROM.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

const int relayNo=3;
#ifdef ESP8266
  const byte relayCh[relayNo]    = {D5, D6, D7};    //{D5, D6, D8}/D7 no, before{D2, D5, D6} //되도록 0(D3),2(D4),15(D8)은 사용하지 마라)-------------------------
#else
  const byte relayCh[relayNo]    = {21, 19, 18};    //{D21, D19, D18} before{D25, D26, D27}
#endif

int charLength=0;
typedef struct _StateData{
  byte mod=0;
  byte req=0;
} StateData;
StateData mngState;

typedef struct _ConfigData{
  char mcuID[12];
  char mcuPASS[12];
  char netID[32];
  char netIP[20];
  char netGATEWAY[20];
  char netSUBNET[20];
  char netPASS[20];
} ConfigData;
ConfigData mngConf;

String wifiJson;

void makemcuID(){
  uint8_t MAC_array[6];
  WiFi.macAddress(MAC_array);  
  sprintf(mngConf.mcuID,"IEUM-%x%x", MAC_array[4], MAC_array[5]);
  mngConf.mcuID[sizeof(mngConf.mcuID) - 1] = '\0';
  //Serial.printf("makemcuID mcuID: %s\n", mngConf.mcuID);
  sprintf(mngConf.mcuPASS,"%s", "00000000"); //mcu 최초 비번(00000000)
  mngConf.mcuPASS[sizeof(mngConf.mcuPASS) - 1] = '\0';
  //Serial.printf("makemcuID password: %s\n",mngConf.mcuPASS);
  writeROM(String(mngConf.mcuID), 0);
  writeROM(String(mngConf.mcuPASS), 12);
  EEPROM.commit();
  yield();
}

AsyncWebServer webServer(80);
AsyncWebSocket webSocket("/ws");
AsyncWebSocketClient* wsClient;
