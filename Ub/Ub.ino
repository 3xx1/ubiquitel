#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Ticker.h>
#include <EEPROM.h>
#include "ub.h"

//IO
const int inA = 4;
const int inB = 5;
const int PS = 12;
const int dock = 13;
const int Vs2B = 14;
const int LED = 16;

int testTap = false;
int setupMode = 0;
int pushed = 0;
unsigned long pushedTime = 0;
int testButton = 0;

Ticker ticker;

void setup() {
  setupEEPROM();
  setupOTA();
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
  udp.begin(6340);
  for (int i = 0; i < 2; i++) {
    numTaps[i] = 0;
    looptime[i] = 0;
  }
}

void loop() {
  if (!setupMode) {
    ArduinoOTA.handle();
    parsePacket();
  }
  checkSetupMode();
}

//タイマ割り込み
void timer() {
  waitForConfirmation();
  if (gtime % 100 == 0) checkDock();
  if (isPlaying) stepTime();
  gtime++;
  if (gtime == playtime) {
    packet[1] = -1;
    playUb();
  }
  checkTestTap();
}

void checkTestTap() {
  if (!digitalRead(testButton) && !testTap && setupMode) {
    testTap = true;
  }
  if (testTap) {
    if (stepCount < 32) {
      stepMotor();
      stepCount++;
    } else if (stepCount < 45) {
      stopMotor();
      stepCount++;
    } else {
      stepCount = 0;
      testTap = false;
    }
  }
}

void checkSetupMode() {
  if (!digitalRead(testButton) && !setupMode) {
    if (!pushed) {
      pushedTime = millis();
      pushed = true;
    } else if ((millis() - pushedTime) > 2000) {
      setupMode = true;
      setupServer();
    }
  }else if(setupMode) {
    loopServer();
  }else {
    pushed = false;
  }
}
