#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Ticker.h>

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

typedef struct note {
  int ts;//Time stamp(Release point)
  int v;//Velocity
  int sp;//Start point
}Note;

typedef enum {
  SYNC_UB,
  PLAY_UB,
  PAUSE_UB,
  STOP_UB,
  SET_LOOP,
  SET_NOTE,
  RESET_NOTE,
  SEARCH_UB
}DataType;

typedef enum {
  UB_FOUND,
  UB_DOCKED,
  UB_UNDOCKED
}CallbackType;

Note notes[32];
int numNotes = 4;
int res = 5;
int now = 0;
int looptime = 1000/res;

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
  
  ticker.attach(0.005, step);
  udp.begin(6340);

  notes[0].ts = 0/res;
  notes[0].v = 40;
  notes[0].sp = notes[0].ts -notes[0].v + looptime;

  notes[1].ts = 250/res;
  notes[1].v = 10;
  notes[1].sp = notes[1].ts -notes[1].v;

  notes[2].ts = 500/res;
  notes[2].v = 30;
  notes[2].sp = notes[2].ts -notes[2].v;

  notes[3].ts = 750/res;
  notes[3].v = 10;
  notes[3].sp = notes[3].ts -notes[3].v;
}

void loop() {
  ArduinoOTA.handle();
  
  int packetSize = udp.parsePacket();
  //親機から受信するデータ
  //SYNC_UB,
  //PLAY_UB,
  //PAUSE_UB,
  //STOP_UB,
  //SET_LOOP,
  //SET_NOTE,
  //RESET_NOTE,
  //SEARCH_UB
    
  if (packetSize){
      tapping = 1;
      rel = udp.read();
  }

  //親機に送信するデータ  
  //UB_FOUND
  //UB_DOCKED
  //UB_UNDOCKED
}

void step() {
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
