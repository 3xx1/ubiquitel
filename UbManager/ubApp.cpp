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
        if(input=='n') {
            upi.rhythmOpen(pnum,unum);
        }
        else if(input=='p') {
            upi.rhythmOpen(pnum,unum);
            char arg[5] = "sync";
            char    *dataList[16];
            upi.split(arg, ", \n", dataList);
            upi.parse(dataList);
        }
        else if(input=='s') {
            r = 0;
            char arg[8] = "allstop";
            char    *dataList[16];
            upi.split(arg, ", \n", dataList);
            upi.parse(dataList);
        }
        else if(input=='q') {
            l = false;
        }
    }
  return 0;
}
