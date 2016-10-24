#include "upi.h"

int main(void) {
    Upi upi;
    bool l = true;
    int r = 0;
    int u = 0;
    int pnum = 14;
    int unum = 5;
    int recordUb = -1;
    char input;
    upi.start();
    printf("enter the UBID to record!\n");
    scanf("%d",&recordUb);
    printf("enter the UNUM!\n");
    scanf("%d",&unum);
    while(upi.ubm.getUbSize()<unum);
    upi.setup(pnum,unum);
    upi.recordUb = recordUb;
    upi.sendCommand("allstop,");
    while(l) {
        if(!upi.isPlaying) {
            usleep(3000000);
            upi.playSong();
        }
        if(!upi.active) l = false;
    }
  return 0;
}
