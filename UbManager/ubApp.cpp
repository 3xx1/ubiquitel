#include "upi.h"

int main(void) {
    Upi upi;
    bool l = true;
    int r = 0;
    int u = 0;
    int pnum = 4;
    int unum = 5;
    char input;
  //upi.start();
    while(l) {
        input = getchar();
        else if(input=='p') {
            upi.setup(pnum,unum);
            upi.playRhythm();
        }
        else if(input=='s') {
            r = 0;
            upi.sendCommand("allstop,");
        }
        else if(input=='q') {
            l = false;
        }
    }
  return 0;
}
