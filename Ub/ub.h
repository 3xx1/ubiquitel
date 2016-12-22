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
  UB_PLAYED
}CallbackType;

typedef struct Tap {
  int ts;//Time stamp(Release point)
  int v;//Velocity
  int sp;//Start point
}Tap;

//Comm
const char* ssid = "MusashiのiPhone";
const char* password = "47440906";
WiFiUDP udp;
IPAddress ubmip;
long packet[1000];
int packetSize = 0;
int waiting[5] = {0,0,0,0,0};
int resendPeriod = 200;
int resend_max = 5;
int resendCount[5] = {resend_max,resend_max,resend_max,resend_max,resend_max};

//Dock
int dockState = HIGH;

//Tap
Tap taps[2][512];
int numTaps[2];
int looptime[2];
int repeat[2];
int rcnt = 0;
int w = 0;
int r = 0;
int vTable[11] = {0,8,9,10,11,12,14,16,22,30,40};
volatile int next = 0;

bool isPlaying = false;
volatile bool tapping = false;
volatile int stepCount = 0;
int now = 0;

//Timer
unsigned long gtime = 0;
int playtime = 0;
int res = 5;
