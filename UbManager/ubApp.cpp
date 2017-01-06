#include "upi.h"

int main(int argc, char **argv) {
    Upi upi;
    int r = 0;
    int u = 0;
    int pnum = 16;
    int unum = 5;
    int recordUb = -1;
    char input;
    char folder[30];
    if(argc != 6) {
        printf("Wrong argument. \"./ubApp [recordUb] [ubNum] [FolderName] [patternNum] [tempo]\"\n");
        return 1;
    }
    
    recordUb = atoi(argv[1]);
    unum = atoi(argv[2]);
    pnum = atoi(argv[4]);
    strcpy(folder, argv[3]);
    
    if(recordUb >= unum) {
        printf("Wrong argument. RecordUb must be smaller than unum.\n");
        return 1;
    }
    while(upi.ubm.getUbSize()<unum);
    upi.setup(pnum,unum);
    upi.recordUb = recordUb;
    upi.coef = atoi(argv[5]);
    upi.start();
    upi.sendCommand("allstop,");
    usleep(1000*1000*1);
    upi.playSong(folder);
    //upi.playSongFrom(folder, 17);
    while(upi.active);
  return 0;
}
