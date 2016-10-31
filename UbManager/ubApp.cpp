#include "upi.h"

int main(int argc, char **argv) {
    Upi upi;
    int r = 0;
    int u = 0;
    int pnum = 14;
    int unum = 5;
    int recordUb = -1;
    char input;
    
    if(argc != 3) {
        printf("Wrong argument. \"./ubApp [recordUb] [ubNum]\"\n");
        return 1;
    }
    
    recordUb = atoi(argv[1]);
    unum = atoi(argv[2]);
    
    if(recordUb >= unum) {
        printf("Wrong argument. RecordUb must be smaller than unum.\n");
        return 1;
    }
    upi.start();
    while(upi.ubm.getUbSize()<unum);
    upi.setup(pnum,unum);
    upi.recordUb = recordUb;
    upi.sendCommand("allstop,");
    while(1) {
        if(!upi.active) break;
        if(!upi.isPlaying) {
            usleep(3000000);
            upi.playSong();
        }
    }
  return 0;
}
