#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UbManager.h"
#define MAX 4

class Upi {
public:
    UbManager ubm;
    char input[256];
    
    Upi() {
        //コールバック関数の登録
        ubm.setCallback(this, &Upi::ubCallback);
        //子機たちのIPアドレス取得
        ubm.search();
    }
    ~Upi(){}
    
    void ubCallback(CallbackType cbt, int ubID){//ユビ状況，ユビID
        int a = 400;
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
            strcpy(input, buf);
            if(ubm.isDocking) {
                char    *dataList[MAX];
                split(buf, ", \n", dataList);
                parse(dataList);
            }else {
                printf("%s **NO UB!**\n", buf);
            }
        }
        pclose(fp);
    }

    void parse(char *dl[]) {
        if(strcmp(dl[0], "add") == 0) {
            int arg[2] = {atoi(dl[1]), atoi(dl[2])};
            ubm.addNote(arg[0]*16.7,arg[1]/20);
        }
        else if(strcmp(dl[0], "sendNotes") == 0) {
            ubm.sendNotes();
        }
        else if(strcmp(dl[0], "sendLoop") == 0) {
            ubm.sendLoop(atoi(dl[1])*16.7);
        }
        else if(strcmp(dl[0], "reset") == 0) {
            ubm.resetNotes();
        }
        else if(strcmp(dl[0], "play") == 0) {
            ubm.playAt(ubm.getDockedUbID(), 0);
        }
        else if(strcmp(dl[0], "stop") == 0) {
            ubm.stop(ubm.getDockedUbID());
        }
        else if(strcmp(dl[0], "sync") == 0) {
            ubm.sync();
        }
        else {
            printf("%s", input);
        }
    }
};