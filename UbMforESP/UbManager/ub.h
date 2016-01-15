#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Ticker.h>

typedef enum {
    SYNC_UB,
    PLAY_UB,
    PAUSE_UB,
    STOP_UB,
    SET_LOOP,
    SET_NOTE,
    RESET_NOTE,
    SEARCH_UB,
    CONFIRM
}DataType;

typedef enum {
    UB_FOUND,
    UB_DOCKED,
    UB_UNDOCKED
}CallbackType;

//Note:ユビを叩くタイミング(0~ループ周期)と強さ(1~10くらい？)を持つ構造体
typedef struct Note {
    long timeStamp;
    long intensity;
}Note;

//Ub:ユビのIPアドレス，ノート，ループ周期(ms)を持つ構造体
typedef struct Ub {
    IPAddress ip;
    long loop;
    Note notes[64];
    long numNotes;
}Ub;
