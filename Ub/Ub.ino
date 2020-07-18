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
