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
  char input[256];
  FILE *rhythm[5][5];
  int nextPattern[PATTERN_MAX] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  int pmax, umax;
  Upi() {
	//コールバック関数の登録
	ubm.setCallback(this, &upi::ubCallback);
	//子機たちのIPアドレス取得
	ubm.search();
  }
  ~Upi(){
	rhythmClose();
  }

  void setup(int patternNum, int ubNum){
	pmax = patternNum;
	umax = ubNum;
  }

  void rhythmOpen(int pattern, int ub) {
	pmax = pattern;
	umax = ub;
        
	for(int r=0;r<pmax;r++) {
	  for(int u=0;u<umax;u++) {
		std::stringstream s;
		std::string filename;
		s << "rhythm" << r << "/rhythm" << r << u << ".txt";
		filename = s.str();
		printf("%s\n", filename.c_str());
		if ((rhythm[r][r] = fopen(filename.c_str(), "r")) == NULL) {
		  printf("FILE OPEN ERROR!!\n");
		}
	  }
	}
  }
    
  void rhythmClose() {
	for(int r=0;r<pmax;r++) {
	  for(int u=0;u<umax;u++) {
		fclose(rhythm[r][u]);
		nextPattern[u] = 0;
	  }
	}
  }
  
  void playSong(int r) {
	sendCommand("allstop,");
	rhythmOpen(pmax,umax);
	for(int i=0;i<umax;i++) {
	  nextPattern[i] = 0;
	  sendCommand(nextPattern[i]++, i);
	}
	sendCommand("sync,");
  }
  
  void playRhythm(int r) {
	sendCommand("allstop,");
	for(int i=0;i<umax;i++) {
	  sendCommand(r, i);
	  nextPattern[i] = pmax;
	}
	sendCommand("sync,");
  }

  void sendCommand(const char* command) {
	  char buf[256];
	  char    *dataList[MAX];
	  strcpy(command, buf);
	  split(buf, ", \n", dataList);
	  parse(dataList);
  }

  void sendCommand(int pattern, int ub) {
	char command[256];
	if(!rhythm[pattern][ub]) printf("no rhythm!");
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
	  if(nextPattern[ubID]<pmax) {
		sendCommand(nextPattern[ubID]++, ubID);
		printf("sent new rhythm,%d!\n",nextPattern[ubID]-1);
	  }
	  else {
		for(int r=0;r<pmax;r++)
		  fclose(rhythm[r][ubID]);
		nextPattern[ubID] = 0;
		printf("stopping rhythm!\n");
	  }
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
	  ubm.addNote(arg[0]*15,arg[1]/20);
	}
	else if(strcmp(dl[0], "record") == 0) {
	  ubm.addNote(ubm.getTimestamp(),dl[1]/20);
	}
	else if(strcmp(dl[0], "sendNotes") == 0) {
	  ubm.sendNotes();
	}
	else if(strcmp(dl[0], "addLoop") == 0) {
	  ubm.addLoop(atoi(dl[1])*15, atoi(dl[2]));
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
	  ubm.resetAll();
	  rhythmClose();
	}
	else if(strcmp(dl[0], "sync") == 0) {
	  ubm.sync();
	}
	else {
	  printf("%s", input);
	}
  }
};
