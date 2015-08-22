// define array size here
static int arraySize = 800;

// array for input signal storage 
int signal[] = new int[arraySize];

int threshold_on  = 30;
int threshold_off = threshold_on;

int pulseInput;

void setup() {
  size(arraySize, 400);
}

void draw() {
  background(255);
  
  /* ******************************************************************* */
  /* take the first line out, switch second line to arduino analog input */ 
  pulseInput = pulseReductionSimulator(pulseInput, .8);
  signal[0] = pulseInput;
  /* ******************************************************************* */
  
  pushArrayData(signal, arraySize);
  osciloLineDraw(signal, arraySize);
}

