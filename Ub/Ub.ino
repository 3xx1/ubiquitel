//使用ライブラリ
//SocketIOClient, Arduino_JSON, WebSockets(by Markus Sattler)
//SocketIoClient.cpp 47行目を以下に置き換えておく
//_webSocket.beginSSL(host, port, url, (uint8_t*)fingerprint);
//最初の書き込みはArduinoを経由してシリアル通信
//基板上のJP2ピンの上→GND,中→D1(TX),下→D0(RX)につなぐ
//電源につないで基板上の上ボタンを押しながら下ボタンを押して、下ボタンを離してから上ボタンを離す
//IDEから書き込む

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Ticker.h>
#include <EEPROM.h>
#include <SocketIoClient.h>
#include <Arduino_JSON.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include "ub.h"

//IO
const int inA = 4;
const int inB = 5;
const int PS = 12;
const int dock = 13;
const int Vs2B = 14;
const int LED = 16;

void setup()
{
  setupEEPROM();
  setupOTA();
  setupSocketIO();

  pinMode(inA, OUTPUT);
  pinMode(inB, OUTPUT);
  pinMode(dock, INPUT_PULLUP);
  pinMode(PS, OUTPUT);
  pinMode(Vs2B, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(testButton, INPUT_PULLUP);

  digitalWrite(inA, LOW);
  digitalWrite(inB, LOW);
  digitalWrite(Vs2B, LOW);
  digitalWrite(PS, HIGH);
  digitalWrite(LED, LOW);

  ticker.attach(0.005, timer);
}

void loop()
{
  if (!setupMode)
  {
    ArduinoOTA.handle();
  }
  webSocket.loop();
  checkSetupMode();
}
