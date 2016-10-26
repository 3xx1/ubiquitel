#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <list>
#include <vector>

typedef enum {
    SYNC_UB,
    PLAY_UB,
    PAUSE_UB,
    STOP_UB,
    SET_LOOP,
    SET_NOTE,
    RESET_NOTE,
    SEARCH_UB,
    CONFIRM
}DataType;

typedef enum {
    UB_FOUND,
    UB_DOCKED,
    UB_UNDOCKED,
    UB_STOPPED,
    UB_PLAYED,
    UB_TIMER
}CallbackType;

//Note:ユビを叩くタイミング(0~ループ周期)と強さ(1~10くらい？)を持つ構造体
typedef struct Note {
    int timeStamp;
    int intensity;
    bool operator < (const Note& note) const {
      if(timeStamp < note.timeStamp) {
	return true;
      }
      return false;
    }
}Note;

//Ub:ユビのIPアドレス，ノート，ループ周期(ms)を持つ構造体
typedef struct Ub {
  char ip[16];
  int loop;
  int repeat;
  std::list<Note> notes;
}Ub;
