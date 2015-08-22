// define array size here
static int arraySize = 800;

// array for input signal storage
int signal[] = new int[arraySize];

int threshold_on  = 30;
int threshold_off = threshold_on;

int pulseInput;
int maxIntensity;
boolean intensityRecognitionEventFlag;

void setup() {
  size(arraySize, 400);
  intensityRecognitionEventFlag = false;
}

void draw() {
  background(255);
  
  /* ******************************************************************* */
  /* take the second line out, switch first line to arduino analog input */
  signal[0] = pulseInput;
  pulseInput = pulseReductionSimulator(pulseInput, .8);
  /* ******************************************************************* */
  
  // TODO: to be cleaned up below
  if(signal[0]>threshold_on && !intensityRecognitionEventFlag) {
    intensityRecognitionEventFlag = true;
    maxIntensity = 0;
  }
  if(intensityRecognitionEventFlag) {
    if(maxIntensity<signal[0]) maxIntensity = signal[0];
  }
  if(signal[0]<threshold_off && intensityRecognitionEventFlag) {
    intensityRecognitionEventFlag = false;
    println(maxIntensity, timestamp());
  }
  if(intensityRecognitionEventFlag) rect(0, 0, 50, 50);
  // TODO: to be cleaned up above
  
  pushArrayData(signal, arraySize);
  osciloLineDraw(signal, arraySize);
}



float timestamp() {
  return frameCount/frameRate; // TODO: replace frameCount with actual app clock later. 
}
