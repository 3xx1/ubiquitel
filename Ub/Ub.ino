#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Ticker.h>
#include "ub.h"

//MotorIO
const int inA = 4;
const int inB = 5;
const int PS = 12;
const int Vs2B = 14;
const int LED = 16;

const char* ssid = "intermediakanno";
const char* password = "kannolab";
WiFiUDP udp;
Ticker ticker;
Note notes[32];
IPAddress ubmip;
bool mfound = false;
bool isPlaying = false;
long packet[1000];
int packetSize = 0;
int numNotes = 4;
int res = 5;
int now = 0;
int looptime = 1000/res;
unsigned long gtime = 0;

volatile int next = 0;
unsigned char rel = 20;
volatile int stepCount = 0;         // number of steps the motor has taken
volatile bool tapping = true;
volatile bool lighting = false;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("End");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(inA, OUTPUT);
  pinMode(inB, OUTPUT);
  pinMode(Vs2B, OUTPUT);
  pinMode(PS, OUTPUT);
  pinMode(LED, OUTPUT);
  
  digitalWrite(inA, LOW);
  digitalWrite(inB, LOW);
  digitalWrite(Vs2B, LOW);
  digitalWrite(PS, HIGH);
  digitalWrite(LED, LOW);
  
  ticker.attach(0.005, timer);
  udp.begin(6341);
}

void loop() {
  ArduinoOTA.handle();
  
  packetSize = udp.parsePacket();
  //親機から受信
  if (packetSize){
      udp.read((char *)packet, sizeof(long)*1000);
  }
  switch(packet[0]) {
    case SYNC_UB:
    gtime = 0;
    break;
    case PLAY_UB:
    if(!isPlaying) isPlaying =true;
    break;
    case PAUSE_UB:
    if(isPlaying) isPlaying =false;
    break;
    case STOP_UB:
    if(isPlaying) isPlaying =false;
    now = notes[0].sp;
    next = 0;
    break;
    case SET_LOOP:
    setLoop();
    break;
    case SET_NOTE:
    setNote();
    break;
    case RESET_NOTE:
    resetNote();
    break;
    case SEARCH_UB:
    mfound = true;
    ubmip = udp.remoteIP();
    sendData(UB_FOUND);
    break;
  }
}

void setLoop() {
  looptime = packet[1];
  for(int i=0;i<numNotes;i++) {
    if(notes[i].sp<0) notes[i].sp += looptime;
  }
}

void setNote() {
  numNotes = (packetSize-1)/2;
  for(int i=0;i<numNotes;i++) {
    notes[i].ts = (int)packet[2*i+1]/res;
    notes[i].v = (int)packet[2*i+2]*4;
    notes[i].sp = notes[i].ts -notes[i].v;
    if(notes[i].sp<0) notes[i].sp += looptime;
  }
}

void resetNote() {
  if(numNotes) {
    for(int i=0;i<numNotes;i++) {
      notes[i].ts = 0;
      notes[i].v = 0;
      notes[i].sp = 0;
    }
  numNotes = 0;
  }
}

void sendData(const CallbackType cbt) {
    CallbackType ubf = cbt;
    udp.beginPacket(ubmip, 6340);
    udp.write((char *)&ubf, sizeof(ubf));
    udp.endPacket();
}

//タイマ割り込み
void timer() {
  if(isPlaying) stepTime();
  gtime++;
}

void stepTime() {
    if(notes[next].sp == now)
      tapping = true;
    
    if(tapping) {
        stepMotor();
        stepCount++;
        if(stepCount == notes[next].v) {
            tapping = false;
            stepCount = 0;
            stopMotor();
            next = (next < numNotes-1) ? next+1 : 0;
        }
    }
    now = (now+1)%looptime;
}

void stepMotor()
{
    int thisStep = stepCount%4;
    digitalWrite(Vs2B, HIGH);
    digitalWrite(PS, LOW);
    digitalWrite(LED, HIGH);
  
    switch (thisStep) {
        case 0:    // 00
            digitalWrite(inA, LOW);
            digitalWrite(inB, LOW);
        break;
        case 1:    // 10
            digitalWrite(inA, HIGH);
            digitalWrite(inB, LOW);
        break;
        case 2:    //11
            digitalWrite(inA, HIGH);
            digitalWrite(inB, HIGH);
        break;
        case 3:    //01
        digitalWrite(inA, LOW);
        digitalWrite(inB, HIGH);
        break;
    }
}

void stopMotor() {
    digitalWrite(PS, HIGH);
    digitalWrite(Vs2B, LOW);
    digitalWrite(LED, LOW);
}
