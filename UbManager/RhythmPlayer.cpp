//
//  RhythmPlayer.cpp
//  
//
//  Created by kannolab1 on 2017/04/26.
//
//

#include "RhythmPlayer.hpp"

RhythmPlayer::RhythmPlayer() {
    rmax = RHYTHM_MAX;
    umax = UB_MAX;
    bpm = 120;
    isPlaying = false;
}

RhythmPlayer::~RhythmPlayer() {
}

void RhythmPlayer::setRhythm(const char *path) {
    //パスをコピー
    strcpy(rPath, path);
    rmax = RHYTHM_MAX;
    umax = UB_MAX;
    //リズムパターンを読み込み
    for(int r=0;r<rmax;r++) {
        for(int u=0;u<umax;u++) {
            std::stringstream s;
            std::string filename;
            s << path << "/rhythm" << r << "/rhythm" << r << u << ".txt";
            filename = s.str();
            if ((rhythm[r][u] = fopen(filename.c_str(), "r")) == NULL) {
                break;
            }
        }
    }
    
    //リズムパターン数とリズムに使われるユビの本数をチェック
    for(int r=0;r<rmax;r++) {
        if (rhythm[r][0] == NULL) {
            rmax = r;
            break;
        }
    }
    for(int u=0;u<umax;u++) {
        if (rhythm[0][u] == NULL) {
            umax = u;
            break;
        }
    }
    //出力
    std::cout << "rmax = " << rmax << ", umax = " << umax << std::endl;
}

void RhythmPlayer::setBPM(int b) {
    bpm = b;
}

void RhythmPlayer::playRhythm(UbManager *ubm, const char *path) {
    if(isPlaying) return;
    setRhythm(path);    //リズム読み込み
    //リズム演奏用ユビたちを停止
    for(int i=0;i<umax;i++) {
        usleep(1000);
        ubm->setDestUbID(i);
        ubm->stop();
        ubm->resetNotes();
        ubm->setDestUbID(-1);
    }
    //リズム演奏用ユビたちをタイマー再生
    for(int i=0;i<umax;i++) {
        nextPattern[i] = 0;
        sendCommand(ubm, nextPattern[i]++, i);
        ubm->setDestUbID(i);
        ubm->setDestUbID(-1);
    }
    ubm->sync();//ユビ同期
    for(int i=0;i<umax;i++) {
        ubm->setDestUbID(i);
        //ubm->playAt(1000);
        ubm->setDestUbID(-1);
    }
    isPlaying = true;
}

void RhythmPlayer::stopRhythm(UbManager *ubm) {
    //ユビに停止命令
    for(int i=0;i<umax;i++) {
        ubm->setDestUbID(i);
        ubm->stop();
        ubm->setDestUbID(-1);
        usleep(1000);
    }
    //ファイルクローズ
    for(int r=0;r<rmax;r++) {
        for(int u=0;u<umax;u++) {
            if(rhythm[r][u]) {
                fclose(rhythm[r][u]);
                rhythm[r][u] = NULL;
            }
            nextPattern[u] = 0;
        }
    }
    isPlaying = false;
    std::cout << "Rhythm stopped" << std::endl;
}

void RhythmPlayer::ubPlayed(UbManager *ubm, int ubID) {
    if(nextPattern[ubID]<rmax && ubID < umax) {
        printf("ub%d required new rhythm!\n", ubID);
        sendCommand(ubm, nextPattern[ubID]++, ubID);
    }else if(ubID < umax){
        printf("ub%d stopping rhythm!\n", ubID);
    }else {
        printf("different ub!\n");
    }
}

void RhythmPlayer::ubStopped(UbManager *ubm, int ubID) {
    if(nextPattern[ubID]==rmax && ubID < umax) {
        printf("ub%d stopped!\n", ubID);
        //nextPatternをインクリメントしてユビがストップした事を示す（nextPattern = rmax+1となる）
        nextPattern[ubID]++;
        //停止したユビをカウント
        int stopUb = 0;
        for(int i=0;i<umax;i++) {
            if(nextPattern[i] == rmax+1) stopUb++;
        }
        //全て停止した時の処理
        if(stopUb == umax) {
            stopRhythm(ubm);
        }
    }else if(ubID < umax){
        printf("ub%d communication err!\n", ubID);
        nextPattern[ubID] = rmax+1;
        stopRhythm(ubm);
    }else {
        printf("different ub!\n");
    }
}

void RhythmPlayer::sendCommand(UbManager *ubm, int pattern, int ub) {
    char command[256];
    if(!rhythm[pattern][ub]) {printf("no rhythm!\n");return;}
    while (fgets(command, 256, rhythm[pattern][ub]) != NULL) {
        char    *dataList[4];
        split(command, ", \n", dataList);
        ubm->setDestUbID(ub);
        parse(ubm, dataList);
        ubm->setDestUbID(-1);
    }
}

int RhythmPlayer::split(char *str, const char *delim, char *outlist[] ) {
    char    *tk;
    int     cnt = 0;
    
    tk = strtok( str, delim );
    while( tk != NULL && cnt < 4 ) {
        outlist[cnt++] = tk;
        tk = strtok( NULL, delim );
    }
    return cnt;
}

void RhythmPlayer::parse(UbManager *ubm, char *dl[]) {
    if(strcmp(dl[0], "add") == 0) {
        int arg[2] = {atoi(dl[1]), atoi(dl[2])};
        ubm->addNote((arg[0]/128.0)*(240.0*1000.0)/(float)bpm,arg[1]/25);
    }
    else if(strcmp(dl[0], "sendNotes") == 0) {
        ubm->sendNotes();
    }
    else if(strcmp(dl[0], "addloop") == 0) {
        ubm->addLoop((atoi(dl[1])/128.0)*(240.0*1000.0)/(float)bpm, atoi(dl[2]));
    }
    else if(strcmp(dl[0], "reset") == 0) {
        ubm->resetNotes();
    }
}

    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
                    
