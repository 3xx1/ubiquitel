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
}CallbackType;

typedef struct Note {
  int ts;//Time stamp(Release point)
  int v;//Velocity
  int sp;//Start point
}Note;

//Comm
const char* ssid = "intermediakanno";
const char* password = "kannolab";
WiFiUDP udp;
IPAddress ubmip;
long packet[1000];
int packetSize = 0;
int waiting[3] = {0,0,0};

//Dock
int dockState = HIGH;

//Note
Note notes[32];
int numNotes = 4;
int vTable[10] = {8,9,10,11,12,14,16,22,30,40};
volatile int next = 0;

//Tap
int looptime = 0;
bool isPlaying = false;
volatile bool tapping = false;
volatile int stepCount = 0;
int now = 0;

//Timer
unsigned long gtime = 0;
int playtime = 0;
int res = 5;



