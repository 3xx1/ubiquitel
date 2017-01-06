// Comment Out If Not Using Arduino
import processing.serial.*;
Serial myPort;

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
int peakTiming;
boolean intensityRecognitionEventFlag;
boolean recordingEventFlag;
boolean playbackEventFlag;
boolean repeatEventFlag;
int clock;
int playback_clock;
int playback_index;

// data storage
ArrayList<Integer> intensity_rec = new ArrayList<Integer>();
ArrayList<Integer> timestamp_rec = new ArrayList<Integer>();

String systemMessage = "press 'r' to start recording, press 'p' to play recorded stuff, press 'x' to repeat it.";

void setup() {
  size(800, 400);
  intensityRecognitionEventFlag = false;
  recordingEventFlag = false;
  playbackEventFlag = false;
  println(systemMessage);
  innerWidth = width - 2*padding;
  myPort = new Serial(this, "/dev/cu.usbmodem1411", 9600);
}

void draw() {
  background(255);
  //grid();
  //rhythm();
  //if(playbackEventFlag) highlight();
  clock++;
  playback_clock++;
  /* ******************************************************************* */
  /* take the second line out, switch first line to arduino analog input */
  if (myPort.available()>0) signal[0] = myPort.read();
  // signal[0] = pulseInput;
  //signal[0] = highlight() * 100;
  pulseInput = pulseReductionSimulator(pulseInput, .8);
  /* ******************************************************************* */

  // drawPeripherals();
  //if(playbackEventFlag)  playbackRecordings(playback_clock, playback_index);
  if(playbackEventFlag) intensityRecognitionEvent(signal, threshold_on, threshold_off);
  pushArrayData(signal, arraySize);
  osciloLineDraw(signal, arraySize);
}