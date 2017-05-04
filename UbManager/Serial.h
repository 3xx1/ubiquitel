#ifndef _Serial_h
#define _Serial_h

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <math.h>
#include <vector>
#include <dirent.h>

#define BAUDRATE B9600           
#define MODEMDEVICE "/dev/cu.usbserial-A50178PJ"
#define _POSIX_SOURCE 1
#define BUF_SIZE 1024
#define FALSE 0
#define TRUE 1

using namespace std;

/*extern "C" {
int read(int handle, void *buf, unsigned n);
int write(int handle, void *buf, unsigned n);
}*/

class Serial {

public:
    int         fd, c, res;
    volatile int active;
	volatile int done;
    pthread_t   thread;
    struct      termios oldtio,newtio;
    char        buf[BUF_SIZE];
	int			rp, wp;
    char        device[128];
    vector<string> deviceList;
    
    Serial();
    ~Serial();
    void setDevice(const char *d);//デバイスのパスを引数に指定し、シリアルデバイスを設定
    int available();
    unsigned char readData();
    void writeData(void *data, int s);

private:
    bool prepareSerial();//シリアル通信の準備
    void threadStop();
    void threadStart();
    static void *threadFunction(void *data);
    
};

#endif
