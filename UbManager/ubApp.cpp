#include "upi.h"

int main(void) {
    Upi upi;
    bool l = true;
    int r = 0;
    int u = 0;
    int pnum = 14;
    int unum = 5;
    char input;
    upi.start();
    while(upi.ubm.getUbSize()!=5);
    upi.setup(pnum,unum);
    while(l) {
        input = getchar();
        if(input=='p') {
            upi.playSong();
        }
        else if(input=='s') {
            r = 0;
            upi.sendCommand("allstop,");
        }
        else if(input=='r') {
            printf("Please enter the number of the rhythm pattern: ");
            scanf("%d",&r);
            upi.playRhythm(r);
        }
        else if(input=='q') {
            upi.stop();
            l = false;
        }
    }
  return 0;
}
