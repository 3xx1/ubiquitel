//
//  ubTap.cpp
//  
//
//  Created by kannolab1 on 2017/04/19.
//
//

#include <stdio.h>
#include "Tap.hpp"
#include "RhythmPlayer.hpp"

#define RECORD_UB 4

class UbTap {
public:
    UbTap() {
        ubm.setCallback(this, &UbTap::ubCallback);//コールバック関数の登録
        ubm.search();//子機たちのIPアドレス取得
        ubm.setTimer(5);//タイマー登録
        while(ubm.getUbSize() != 5);
        rhythm.setBPM(160);
        tap.setRecordUb(RECORD_UB);
        stoppingTime = ubm.getTimestamp();
    };
    ~UbTap(){};
    void stop() {
        rhythm.stopRhythm(&ubm);
        tap.stopTaps(&ubm);
    };
    
    UbManager ubm;
    Tap tap;
    RhythmPlayer rhythm;
    int stoppingTime;
    
    void ubCallback(CallbackType cbt, int ubID){//ユビ状況，ユビID
        switch(cbt) {
            case UB_FOUND://見つかった時
                printf("ub%d found!\n", ubID);
                break;
                
            case UB_DOCKED://ドッキングした時
                printf("ub%d docked!\n", ubID);
                break;
                
            case UB_UNDOCKED://ドッキング解除した時
                printf("ub%d undocked!\n", ubID);
                break;
                
            case UB_PLAYED://リズムデータ要求
                printf("ub%d played!\n", ubID);
                if(ubID != RECORD_UB)
                    rhythm.ubPlayed(&ubm, ubID);
                else tap.rhythmIsReceived();
                break;
                
            case UB_STOPPED://ユビストップ時
                rhythm.ubStopped(&ubm, ubID);
                //リズムが停止したらその時間を記録
                if(!rhythm.isPlaying) {
                    stoppingTime = ubm.getTimestamp();
                }
                break;
                
            case UB_TIMER://指定したミリ秒経過時
                tap.intensityRecognitionEvent(&ubm);
                //リズム停止後2秒経過したらリズム再生
                if(!rhythm.isPlaying && (ubm.getTimestamp() - stoppingTime) > 500) {
                    rhythm.playRhythm(&ubm, "./fingerwerk4");
                }
                break;
        }
    };
};

int main(int argc, char **argv) {
    UbTap ubtap;
    char s;
    bool q = false;
    
    while(!q) {
        scanf("%c", &s);
        switch (s) {
            case 'q':
                q = true;
                break;
            case 'p':
                ubtap.rhythm.playRhythm(&(ubtap.ubm), "./fingerwerk4");
                break;
            case 's':
                ubtap.rhythm.stopRhythm(&(ubtap.ubm));
                ubtap.stoppingTime = ubtap.ubm.getTimestamp();
                break;
            case 'y':
                ubtap.ubm.sync();
            default:
                break;
        }
    }
    ubtap.stop();
}

