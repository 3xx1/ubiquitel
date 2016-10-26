#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include "UbManager.h"
#define MAX 4
#define PATTERN_MAX 15
#define UB_MAX 5

class Upi {
public:
    UbManager ubm;
    pthread_t thread;
    bool repeat =false;
    bool isPlaying = false;
    int isRecording = 0;
    int active = 0;
    char input[256];
    FILE *rhythm[PATTERN_MAX][UB_MAX];
    int nextPattern[PATTERN_MAX] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int pmax, umax;
    int lastNote = -1;
    int recordUb = -1;
    Upi() {
        //コールバック関数の登録
        ubm.setCallback(this, &Upi::ubCallback);
        //子機たちのIPアドレス取得
        ubm.search();
        ubm.setTimer(128*13);
    }
    ~Upi(){
        rhythmClose();
    }
    
    void setup(int patternNum, int ubNum){
        pmax = patternNum;
        umax = ubNum;
        for(int r=0;r<pmax;r++) {
            for(int u=0;u<umax;u++) {
                rhythm[r][u] = NULL;
            }
        }
    }
    
    void rhythmOpen() {
        for(int r=0;r<pmax;r++) {
            for(int u=0;u<umax;u++) {
                std::stringstream s;
                std::string filename;
                s << "rhythm" << r << "/rhythm" << r << u << ".txt";
                filename = s.str();
                if ((rhythm[r][u] = fopen(filename.c_str(), "r")) == NULL) {
                    printf("FILE OPEN ERROR!!\n");
                }
            }
        }
    }
    
    void rhythmClose() {
        for(int r=0;r<pmax;r++) {
            for(int u=0;u<umax;u++) {
                if(rhythm[r][u]) {
                    fclose(rhythm[r][u]);
                    rhythm[r][u] = NULL;
                }
                nextPattern[u] = 0;
            }
        }
    }
    
    void playSong() {
        if(isPlaying) return;
        rhythmOpen();
        for(int i=0;i<umax;i++) {
            if(i != recordUb) {
                nextPattern[i] = 0;
                sendCommand(nextPattern[i]++, i);
            }else {
                sendCommand("reset,",i);
                sendCommand("addloop,127,0,", i);
                sendCommand("add,0,0,", i);
                sendCommand("sendNotes,", i);
            }
        }
        sendCommand("sync,");
        isPlaying = true;
    }
    
    void playRhythm(int r) {
        if(isPlaying) return;
        //sendCommand("allstop,");
        rhythmOpen();
        for(int i=0;i<umax;i++) {
            sendCommand(r, i);
            nextPattern[i] = pmax;
        }
        sendCommand("sync,");
        isPlaying = true;
    }
    
    void sendCommand(const char* command) {
        char buf[256];
        char    *dataList[MAX];
        strcpy(buf, command);
        split(buf, ", \n", dataList);
        parse(dataList);
    }
    
    void sendCommand(const char* command, int ub) {
        char buf[256];
        char    *dataList[MAX];
        strcpy(buf, command);
        split(buf, ", \n", dataList);
        ubm.setDestUbID(ub);
        parse(dataList);
        ubm.setDestUbID(-1);
    }
    
    void sendCommand(int pattern, int ub) {
        char command[256];
        if(!rhythm[pattern][ub]) {printf("no rhythm!\n");return;}
        while (fgets(command, 256, rhythm[pattern][ub]) != NULL) {
            char    *dataList[MAX];
            ubm.setDestUbID(ub);
            split(command, ", \n", dataList);
            parse(dataList);
            ubm.setDestUbID(-1);
        }
    }
    
    void ubCallback(CallbackType cbt, int ubID){//ユビ状況，ユビID
        int a = 400;
        bool allstop;
        
        switch(cbt) {
            case UB_FOUND://見つかった時
                printf("ub%d found!\n", ubID);
                //n = ubID;
                break;
                
            case UB_DOCKED://ドッキングした時
                printf("ub%d docked!\n", ubID);
                
                break;
                
            case UB_UNDOCKED://ドッキング解除した時
                printf("ub%d undocked!\n", ubID);
                break;
                
            case UB_PLAYED://リズムデータ要求
                //printf("ub%d required new rhythm!\n", ubID);
                if(nextPattern[ubID]<pmax && ubID < umax && ubID != recordUb) {
                    sendCommand(nextPattern[ubID]++, ubID);
                    //printf("sent new rhythm,%d-%d!\n",nextPattern[ubID]-1,ubID);
                }
                else if(repeat) {
                    nextPattern[ubID] = 0;
                    for(int r=0;r<pmax;r++) {
                        fclose(rhythm[r][ubID]);
                        std::stringstream s;
                        std::string filename;
                        s << "rhythm" << r << "/rhythm" << r << ubID << ".txt";
                        filename = s.str();
                        if ((rhythm[r][ubID] = fopen(filename.c_str(), "r")) == NULL) {
                            printf("FILE OPEN ERROR!!\n");
                        }
                    }
                    sendCommand(nextPattern[ubID]++, ubID);
                    //printf("sent new rhythm,%d-%d!\n",nextPattern[ubID]-1,ubID);
                }
                else if(ubID == recordUb) {
                    /*if(ubm.loopCount%2 == 1 && ubm.getNoteSize(recordUb) > 0) {
                        ubm.setDestUbID(recordUb);
                        ubm.sendNotes(0, 128*13);
                        printf("send former! %d\n", ubm.loopCount);
                        ubm.setDestUbID(-1);
                    }
                    else if(ubm.loopCount%2 == 0 && ubm.getNoteSize(recordUb) > 0) {
                        ubm.setDestUbID(recordUb);
                        ubm.sendNotes(128*13, 256*13);
                        printf("send latter %d\n", ubm.loopCount);
                        ubm.setDestUbID(-1);
                    }
                    else {
                        ubm.setDestUbID(recordUb);
                        ubm.addNote(0,0);
                        printf("send no note %d\n", ubm.loopCount);
                        ubm.setDestUbID(-1);
                    }*/
                }
                else {
                    printf("stopping rhythm!\n");
                }
                break;
                
            case UB_STOPPED://ユビストップ時
                if(ubID >= umax) break;
                printf("ub%d stopped!\n", ubID);
                for(int r=0;r<pmax;r++)
                    if(rhythm[r][ubID]) fclose(rhythm[r][ubID]);
                nextPattern[ubID] = 0;
                allstop = true;
                for(int u=0;u<umax;u++) {
                    if(u != recordUb && nextPattern[u]>0) allstop=false;
                }
                if(allstop) {
                    ubm.stopAll();
                    ubm.resetAll();
                    rhythmClose();
                    isPlaying = false;
                }
                break;
                
            case UB_TIMER:
                //printf("Hey!%d\n", ubm.loopCount);
                if(recordUb < 0) break;
                usleep(100000);
                if(ubm.loopCount%2 == 1 && ubm.getNoteSize(recordUb) > 0) {
                    ubm.setDestUbID(recordUb);
                    ubm.sendNotes(0, 128*13);
                    //printf("send former! %d\n", ubm.getNoteSize(recordUb));
                    ubm.setDestUbID(-1);
                }
                else if(ubm.loopCount%2 == 0 && ubm.getNoteSize(recordUb) > 0) {
                    ubm.setDestUbID(recordUb);
                    ubm.sendNotes(128*13, 256*13);
                    //printf("send latter %d\n", ubm.getNoteSize(recordUb));
                    ubm.setDestUbID(-1);
                }
                
                if(ubm.getTimestamp() > 90000) {
                    printf("force stop\n");
                    sendCommand("allstop,");
                }
                if(isRecording > 0)   isRecording--;

                break;
        }
    }
    
    int split( char *str, const char *delim, char *outlist[] ) {
        char    *tk;
        int     cnt = 0;
        
        tk = strtok( str, delim );
        while( tk != NULL && cnt < MAX ) {
            outlist[cnt++] = tk;
            tk = strtok( NULL, delim );
        }
        return cnt;
    }
    
    void start() {
        int result;
        
        if (!active) {
            active = 1;
            result = pthread_create(&thread, NULL, threadFunction, (void *)this);
        }
        
    }
    
    void stop() {
        int result;
        
        if (active) {
            active = 0;
            result = pthread_join( thread, NULL );
        }
    }
    
    static void *threadFunction(void *data) {
        Upi *upi = (Upi *)data;
        char buf[256];//p5アプリのパス格納とp5アプリからのデータ格納用
        
        //p5アプリのパス設定
        FILE *pwd = popen("pwd", "r");
        fgets(buf,sizeof(buf),pwd);
        pclose(pwd);
        
        char *fn = strstr(buf, "UbManager");
        strcpy(fn, "ubiquitel_processing_app/ubiquitel_beatRecognition/application.macosx/ubiquitel_beatRecognition.app/Contents/MacOS/ubiquitel_beatRecognition");
        
        //processingアプリ起動
        FILE *fp = popen(buf, "r");
        while(fgets(buf,sizeof(buf),fp)) {
            strcpy(upi->input, buf);
            char    *dataList[MAX];
            upi->split(buf, ", \n", dataList);
            upi->ubm.setDestUbID(upi->recordUb);
            upi->parse(dataList);
            upi->ubm.setDestUbID(-1);
        }
        upi->ubm.stopAll();
        upi->ubm.resetAll();
        upi->rhythmClose();
        upi->isPlaying = false;
        
        pclose(fp);
        upi->stop();
        pthread_exit(NULL);
    }
    
    void parse(char *dl[]) {
        if(strcmp(dl[0], "add") == 0) {
            int arg[2] = {atoi(dl[1]), atoi(dl[2])};
            ubm.addNote(arg[0]*13,arg[1]/20);
        }
        else if(strcmp(dl[0], "record") == 0) {
            if(!isRecording) {
                isRecording = 2;
                ubm.resetNotes();
                ubm.addLoop(128*13, 0);
            }
            int a = (ubm.getTimestamp()-150)%(256*13);
            int b = (16*13)*(a/(16*13));//16分音符クオンタイズ
            if(lastNote != b) {
                ubm.addNote(b,atoi(dl[1])/25);
                printf("note:%d, %d\n",b,atoi(dl[1])/25);
                lastNote = b;
            }
        }
        else if(strcmp(dl[0], "sendNotes") == 0) {
            ubm.sendNotes();
        }
        else if(strcmp(dl[0], "addloop") == 0) {
            ubm.addLoop(atoi(dl[1])*13, atoi(dl[2]));

        }
        else if(strcmp(dl[0], "reset") == 0) {
            ubm.resetNotes();
        }
        else if(strcmp(dl[0], "play") == 0) {
            playSong();
        }
        else if(strcmp(dl[0], "stop") == 0) {
            ubm.stop();
        }
        else if(strcmp(dl[0], "allstop") == 0) {
            ubm.stopAll();
            ubm.resetAll();
            rhythmClose();
            isPlaying = false;
        }
        else if(strcmp(dl[0], "sync") == 0) {
            ubm.sync();
        }
        else if(strcmp(dl[0], "search") == 0) {
            ubm.search();
        }
        else {
            printf("%s", input);
        }
    }
};
