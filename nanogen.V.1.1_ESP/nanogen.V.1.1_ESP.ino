/*
  author : jus.kim
  date : 2022.11.11
*/
#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266mDNS.h>
  #include <FS.h>
#else
  #include <WiFi.h>
  #include <WebServer.h>
  #include <ESPmDNS.h>
  #include <SPIFFS.h>
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
  const byte relayCh[relayNo]    = {21, 19, 18};    //{21, 19, 18} before{25, 26, 27}
#endif
const int waterPump=0;
const int airPump=1;
const int solenoid=2;

int charLength=0;
typedef struct _StateData{
  byte mod=0;
  byte req=0;
  char nx2mcu[6];
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
