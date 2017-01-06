#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include "UbManager.h"
#define MAX 4
#define PATTERN_MAX 30
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
    float coef = 15975.0/(3.0*256.0);

    Upi() {
        //コールバック関数の登録
        ubm.setCallback(this, &Upi::ubCallback);
        //子機たちのIPアドレス取得
        ubm.search();
        ubm.setTimer(128*coef);
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
    
    void rhythmOpen(const char *path) {
        for(int r=0;r<pmax;r++) {
            for(int u=0;u<umax;u++) {
                std::stringstream s;
                std::string filename;
                s << path << "/rhythm" << r << "/rhythm" << r << u << ".txt";
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
    
    void playSong(const char *path) {
        if(isPlaying) return;
        rhythmOpen(path);
        for(int i=0;i<umax;i++) {
            if(i != recordUb) {
                nextPattern[i] = 0;
                sendCommand(nextPattern[i]++, i);
            }else {
                sendCommand("reset,",i);
                sendCommand("addloop,128,1,", i);
		ubm.setDestUbID(i);
                ubm.sendEmptyLoop();
		ubm.setDestUbID(-1);
                nextPattern[i] = 0;
            }
        }
        sendCommand("sync,");
        isPlaying = true;
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
                sendCommand("addloop,128,1,", i);
                ubm.setDestUbID(i);
                ubm.sendEmptyLoop();
                ubm.setDestUbID(-1);
                nextPattern[i] = 0;
            }
        }
        sendCommand("sync,");
        isPlaying = true;
    }
    
    void playSongFrom(int pn) {
        if(isPlaying) return;
        rhythmOpen();
        for(int i=0;i<umax;i++) {
            if(i != recordUb) {
                nextPattern[i] = pn;
                sendCommand(nextPattern[i]++, i);
            }else {
                sendCommand("reset,",i);
                sendCommand("addloop,128,1,", i);
                ubm.setDestUbID(i);
                ubm.sendEmptyLoop();
                ubm.setDestUbID(-1);
                nextPattern[i] = 0;
            }
        }
        sendCommand("sync,");
        isPlaying = true;
    }
    
    void playSongFrom(const char *path, int pn) {
        if(isPlaying) return;
        rhythmOpen(path);
        for(int i=0;i<umax;i++) {
            if(i != recordUb) {
                nextPattern[i] = pn;
                sendCommand(nextPattern[i]++, i);
            }else {
                sendCommand("reset,",i);
                sendCommand("addloop,128,1,", i);
                ubm.setDestUbID(i);
                ubm.sendEmptyLoop();
                ubm.setDestUbID(-1);
                nextPattern[i] = 0;
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
                printf("ub%d required new rhythm!\n", ubID);
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
		  /*ネクストパターンを次のデータ送信可能フラグとして使用
		  printf("play!\n");
		  //nextPattern[recordUb] = 1;
		  if(nextPattern[recordUb] == 1) {
		    //if(!nextPattern[recordUb]) printf("Data will be rejected!\n");
		    ubm.setDestUbID(recordUb);
		    ubm.sendNotes(0, 128*coef);
		    printf("send former! %d\n", ubm.getNoteSize(recordUb));
		    ubm.setDestUbID(-1);
		    nextPattern[recordUb] = 0;
		  }
		  else if(nextPattern[recordUb] == 0) {
		    //if(!nextPattern[recordUb]) printf("Data will be rejected!\n");
		    ubm.setDestUbID(recordUb);
		    ubm.sendNotes(128*coef, 256*coef);
		    printf("send latter %d\n", ubm.getNoteSize(recordUb));
		    ubm.setDestUbID(-1);
		    nextPattern[recordUb] = 1;
		    }*/
                }
                else {
		  printf("ub%d stopping rhythm!\n", ubID);
                }
                break;
                
            case UB_STOPPED://ユビストップ時
                if(ubID >= umax) break;
                if((ubID != recordUb && nextPattern[ubID] != pmax) ||
		   ubID == recordUb) {
		  sendCommand("allstop,");
		  //playSongFrom(nextPattern[ubID]);
		  std::cout << "Communication err." << std::endl;
                }
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
	      if(ubm.getTimestamp() > 185000) {
		printf("force stop\n");
		sendCommand("allstop,");
	      }
	      //printf("timer:%d\n",ubm.getTimestamp());
	      if(recordUb < 0 || isPlaying == false) break;
	      /*usleep(200000);
	      if(ubm.loopCount%2 == 1) {
		//再生バッファが空になってから次のデータを
		//送らないとリジェクトされる可能性あり
		if(!nextPattern[recordUb]) printf("Data will be rejected!\n");
		ubm.setDestUbID(recordUb);
		ubm.sendNotes(0, 128*coef);
		printf("send former! %d\n", ubm.getNoteSize(recordUb));
		ubm.setDestUbID(-1);
		nextPattern[recordUb] = 0;
	      }
	      else if(ubm.loopCount%2 == 0) {
		if(!nextPattern[recordUb]) printf("Data will be rejected!\n");
		ubm.setDestUbID(recordUb);
		ubm.sendNotes(128*coef, 256*coef);
		printf("send latter %d\n", ubm.getNoteSize(recordUb));
		ubm.setDestUbID(-1);
		nextPattern[recordUb] = 0;
		}*/
	      //if(isRecording>0) isRecording--;
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
        strcpy(fn, "ubiquitel_processing_app_Rhythmer/ubiquitel_beatRecognition/application.macosx/ubiquitel_beatRecognition.app/Contents/MacOS/ubiquitel_beatRecognition");
        
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
            ubm.addNote(arg[0]*coef,arg[1]/25);
        }
	else if(strcmp(dl[0],"rstart") == 0) {
	  ubm.resetNotes();
	  isRecording = ubm.getTimestamp();
	}
	else if(strcmp(dl[0],"rstop") == 0) {
	  ubm.addLoop(ubm.getTimestamp()-isRecording, 0);
	  ubm.sendNotes();
	  ubm.play();
	  std::cout << ubm.getTimestamp()-isRecording << std::endl;
	}
        else if(strcmp(dl[0], "record") == 0) {
            if(isRecording==0) {
	      //isRecording = 3;
	      //ubm.resetNotes();
            }
            int a = ubm.getTimestamp()-isRecording;
	    ubm.addNote(a,atoi(dl[1])/25);
	    printf("record, %d\n",a);
        }
        else if(strcmp(dl[0], "sendNotes") == 0) {
            ubm.sendNotes();
        }
        else if(strcmp(dl[0], "addloop") == 0) {
            ubm.addLoop(atoi(dl[1])*coef, atoi(dl[2]));
        }
        else if(strcmp(dl[0], "reset") == 0) {
            ubm.resetNotes();
        }
        else if(strcmp(dl[0], "play") == 0) {
            playSong(dl[1]);
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
        else if(strcmp(dl[0], "recordUbID") == 0) {
            recordUb = atoi(dl[1]);
	    printf("rID:%d\n",recordUb);
        }
        else {
            printf("%s", input);
        }
    }
};
