//
//  RhythmPlayer.hpp
//  
//
//  Created by kannolab1 on 2017/04/26.
//
//

#ifndef RhythmPlayer_hpp
#define RhythmPlayer_hpp

#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>
#include "UbManager.h"
#define RHYTHM_MAX 30
#define UB_MAX 10

class RhythmPlayer {
public:
    int rmax, umax, nextPattern[UB_MAX], bpm;
    bool isPlaying;
    char rPath[30];
    FILE *rhythm[RHYTHM_MAX][UB_MAX];
    RhythmPlayer();
    ~RhythmPlayer();
    void setRhythm(const char *path);//リズムの読込
    void playRhythm(UbManager *ubm, const char *path);//リズム再生
    void stopRhythm(UbManager *ubm);//リズム停止
    void setBPM(int b);//テンポの変更
    void ubPlayed(UbManager *ubm, int ubID);
    void ubStopped(UbManager *ubm, int ubID);
    void sendCommand(UbManager *ubm, int pattern, int ub);
    int split(char *str, const char *delim, char *outlist[] );
    void parse(UbManager *ubm, char *dl[]);
};

#endif /* RhythmPlayer_hpp */
