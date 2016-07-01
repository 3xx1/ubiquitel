void drawPeripherals() {
  if (recordingEventFlag) {
    stroke(255,0,0);
    rect(width-10, 0, 10, 10);
  }
  
  if (playbackEventFlag) {
    stroke(0,255,0);
    rect(width-20, 0, 10, 10);
  }
}

void pushArrayData(int[] signal, int arraySize) {
  for(int i=1; i<arraySize; i++) {
    signal[arraySize-i] = signal[arraySize-i-1];
  }
}

void osciloLineDraw(int[] signal, int arraySize) {
  stroke(0);
  for(int i=0; i<arraySize-1; i++) {
    line(i, height - signal[i], i+1, height - signal[i+1]);
  }
}