#include "upi.h"

int main(int argc, char **argv) {
    Upi upi;
    int r = 0;
    int u = 0;
    int pnum = 1;
    int unum = 1;
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
    while(upi.active);
    return 0;
}
