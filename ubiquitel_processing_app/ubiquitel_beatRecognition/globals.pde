// define array size here
static int arraySize = 800;

// array for input signal storage
int signal[] = new int[arraySize];

// threshold values for intensityRecognitionEvent
int threshold_on  = 30;
int threshold_off = 30;

// global valuables 
int pulseInput;
int maxIntensity;
boolean intensityRecognitionEventFlag;
boolean recordingEventFlag;
boolean playbackEventFlag;
int clock;
int playback_clock;
int playback_index;

// data storage
ArrayList<Integer> intensity_rec = new ArrayList<Integer>();
ArrayList<Integer> timestamp_rec = new ArrayList<Integer>();

String systemMessage = "press 'r' to start recording, press 'p' to play recorded stuff.";

void setup() {
  size(arraySize, 400);
  intensityRecognitionEventFlag = false;
  recordingEventFlag = false;
  playbackEventFlag = false;
  println(systemMessage);
  // String portName = Serial.list()[0];
  // myPort = new Serial(this, portName, 9600);
  
}

