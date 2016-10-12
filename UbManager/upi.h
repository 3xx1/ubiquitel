#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sstream>
#include "UbManager.h"
#define MAX 4

class Upi {
public:
    UbManager ubm;
    char input[256];
    FILE *rhythm[5][5];
    
    Upi() {
        //コールバック関数の登録
        ubm.setCallback(this, &Upi::ubCallback);
        //子機たちのIPアドレス取得
        ubm.search();
    }
    ~Upi(){}
    
    void rhythmOpen() {
        int r = 0;
        int u = 0;
        char command[256];
        
        std::stringstream s;
        std::string fileName;
        s << "rhythm" << r << u << ".txt";
        fileName = s.str();
        printf("%s\n", fileName.c_str());

        if ((rhythm[r][u] = fopen(fileName.c_str(), "r")) == NULL) {
            printf("file open error!!\n");
            exit(EXIT_FAILURE);	/* (3)エラーの場合は通常、異常終了する */
        }
        
        while (fgets(command, 256, rhythm[r][u]) != NULL) {
            printf("%s", command);
            char    *dataList[MAX];
            split(command, ", \n", dataList);
            parse(dataList);
        }
        fclose(rhythm[r][u]);	/* (5)ファイルのクローズ */
    }
    
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
                
            case UB_PLAYED://リズムデータ要求
                printf("ub%d required new rhythm!\n", ubID);
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
            char    *dataList[MAX];
            split(buf, ", \n", dataList);
            parse(dataList);
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
        else if(strcmp(dl[0], "addLoop") == 0) {
            ubm.addLoop(atoi(dl[1])*16.7, atoi(dl[2]));
        }
        else if(strcmp(dl[0], "reset") == 0) {
            ubm.resetNotes();
        }
        else if(strcmp(dl[0], "play") == 0) {
            ubm.playAt(0);
        }
        else if(strcmp(dl[0], "stop") == 0) {
            ubm.stop();
        }
        else if(strcmp(dl[0], "allstop") == 0) {
            ubm.stopAll();
        }
        else if(strcmp(dl[0], "sync") == 0) {
            ubm.sync();
        }
        else {
            printf("%s", input);
        }
    }
};
