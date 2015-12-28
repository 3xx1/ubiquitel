#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Ticker.h>

const char* ssid = "intermediakanno";
const char* password = "kannolab";
WiFiUDP udp;
Ticker ticker;

//Ubi1
const int inA = 4;
const int inB = 5;
const int PS = 12;
const int Vs2B = 14;
const int LED = 16;

unsigned char rel = 20;
volatile int waiting = 5;
volatile int stepCount = 0;         // number of steps the motor has taken
volatile int tapping = 0;
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
  
  ticker.attach(0.005, step);
  udp.begin(6340);
}

void loop() {
    ArduinoOTA.handle();
  
    int packetSize = udp.parsePacket();
    if (packetSize){
        tapping = 1;
        rel = udp.read();
    }
  
    //udp.beginPacket("10.0.1.6", 6340);
    //udp.write("58");
    //udp.endPacket();
}

void step() {
    if(tapping) {
        stepMotor();
        stepCount++;
        if(stepCount == rel) {
            tapping = 0;
            stepCount = 0;
            waiting = 5;
        }
    }else if(!tapping) {
        if(waiting > 0) waiting--;
        else stopMotor();
    }
}

void led() {
  if(lighting) digitalWrite(LED, HIGH);
  else digitalWrite(LED, LOW);
  lighting = !lighting;
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
