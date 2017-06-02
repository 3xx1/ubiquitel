//
//  Tap.cpp
//  
//
//  Created by kannolab1 on 2017/04/19.
//
//

#include "Tap.hpp"

Tap::Tap() {
    recordUb = -1;
    signal = 0;
    clock = 0;
    peakTiming = 0;
    maxIntensity = 0;
    threshold_on  = 30;
    threshold_off = 30;
    tapsGetReady = false;
    isPlaying = false;
    serial.setDevice("/dev/cu.usbmodem1421");
}

Tap::~Tap() {
    
}

void Tap::setRecordUb(int rUb) {
    recordUb = rUb;
}

void Tap::intensityRecognitionEvent(UbManager *ubm) {
    unsigned char tmp[4];
    if(serial.available()>3)  {
        tmp[0] = serial.readData();
        tmp[1] = serial.readData();
        tmp[2] = serial.readData();
        tmp[3] = serial.readData();
        signal = tmp[3];
    }
    //立ち上がり検出
    if(signal>threshold_on && !intensityRecognitionEventFlag) {
        intensityRecognitionEventFlag = true;
        maxIntensity = 0;
        //録音フラグを立てて再生中のユビを止める
        if(!recordingEventFlag) {
            recordingEventFlag = true;
            stopTaps(ubm);
        }
        
    }
    
    //ピーク値検出
    if(intensityRecognitionEventFlag) {
        if(maxIntensity<signal) {
            maxIntensity = signal;
            peakTiming = clock*5;
        }
    }
    
    //立ち下がり検出
    if(signal<threshold_off && intensityRecognitionEventFlag) {
        intensityRecognitionEventFlag = false;
        if (recordingEventFlag) {
            if(taps.size()<1) {
                clock = 0;
                peakTiming = 0;
            }
            Note tap;
            tap.intensity = maxIntensity/27;
            tap.timeStamp = peakTiming;
            taps.push_back(tap);
            printf("tapped! clock:%d\n", peakTiming);
        }
    }
    
    //タップ終了検出
    auto itr = taps.end();itr--;
    int lastTimestamp = (*itr).timeStamp;
    if(recordingEventFlag && (clock*5 - lastTimestamp > 2000)) {
        tapsGetReady = true;
        recordingEventFlag = false;
        loop = lastTimestamp + 1000;
        printf("Recording stopped! loop:%d\n",loop);
        playTaps(ubm);
    }
    if(recordingEventFlag) clock++;
}

std::list<Note> Tap::getTaps() {
    return taps;
}

int Tap::getLoop() {
    return loop;
}

void Tap::resetTaps() {
    taps.clear();
    clock = 0;
    tapsGetReady = false;
    serial.setDevice("/dev/cu.usbmodem1421");
}

void Tap::playTaps(UbManager *ubm) {
    if(tapsGetReady) {
        ubm->setDestUbID(recordUb);
        ubm->addNotes(taps);
        ubm->addLoop(loop, 0);
        ubm->sendNotes();
        ubm->play();
        ubm->setDestUbID(-1);
    }
}

void Tap::stopTaps(UbManager *ubm) {
    ubm->setDestUbID(recordUb);
    ubm->stop();
    ubm->resetNotes();
    ubm->setDestUbID(-1);
    isPlaying = false;
    printf("stop recording UB!\n");
}

void Tap::rhythmIsReceived() {
    isPlaying = true;
    resetTaps();
}