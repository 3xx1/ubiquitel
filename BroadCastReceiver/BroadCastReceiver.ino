#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#define out 12
#define in 14

const char* ssid = "intermediakanno";
const char* password = "kannolab";
WiFiUDP udp;
unsigned long w = 0;
unsigned long e = 0;

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

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

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

  pinMode(out, OUTPUT);
  pinMode(in,  INPUT);
  digitalWrite(out, LOW);
  
  udp.begin(6340);
}

void loop() {
  ArduinoOTA.handle();
  int packetSize = udp.parsePacket();
  if (packetSize){
    w = micros();
    digitalWrite(out, HIGH);
    udp.flush();
  }
  
  if(digitalRead(in) == HIGH) e = micros();
  
  if(w && e) {
    long d = e-w;
    bool plus = true;
    if (d<0) {
      d = -d;
      plus = false;
    }
    
    delay(100);
    udp.beginPacket("10.0.1.6", 6340);
    if(plus) udp.write("+");
    else udp.write("-");
    udp.write(d/1000+48);
    udp.write((d%1000)/100+48);
    udp.write((d%100)/10+48);
    udp.write((d%100)%10+48);
    udp.write("us, from ");
    udp.write("58");
    udp.endPacket();
    reset();
  }
}

void reset() {
    digitalWrite(out, LOW);
    delay(10);
    w = 0;
    e = 0;
}

