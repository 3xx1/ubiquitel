//
//  Tap.hpp
//  
//
//  Created by kannolab1 on 2017/04/19.
//
//

#ifndef Tap_hpp
#define Tap_hpp

#include <stdio.h>
#include "Serial.h"
#include "UbManager.h"

class Tap {
public:
    Tap();
    ~Tap();
    void resetTaps();
    void intensityRecognitionEvent(UbManager *ubm);
    std::list<Note> getTaps();
    int getLoop();
    void setRecordUb(int rUb);
    void playTaps(UbManager *ubm);
    void stopTaps(UbManager *ubm);
    void rhythmIsReceived();
    bool tapsGetReady, isPlaying;

private:
    Serial serial;
    std::list<Note> taps;
    int signal;
    int clock;
    int loop;
    int recordUb;
    int peakTiming;
    int maxIntensity;
    int threshold_on;
    int threshold_off;
    bool recordingEventFlag;
    bool intensityRecognitionEventFlag;
};
#endif /* Tap_hpp */
