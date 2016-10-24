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
        if(!upi.isPlaying) {
            usleep(3000000);
            upi.playSong();
        }
    }
  return 0;
}
