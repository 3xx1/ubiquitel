// import processing.serial.*;
// Serial myPort;

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


void setup() {
  size(arraySize, 400);
  intensityRecognitionEventFlag = false;
  // String portName = Serial.list()[0];
  // myPort = new Serial(this, portName, 9600);
  
}

void draw() {
  background(255);
  
  /* ******************************************************************* */
  /* take the second line out, switch first line to arduino analog input */
  // if (myPort.available()>0) signal[0] = myPort.read();
  signal[0] = pulseInput;
  pulseInput = pulseReductionSimulator(pulseInput, .8);
  /* ******************************************************************* */
  
  intensityRecognitionEvent(signal, threshold_on, threshold_off);
  pushArrayData(signal, arraySize);
  osciloLineDraw(signal, arraySize);
}

