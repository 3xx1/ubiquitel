void drawPeripherals() {
  if (recordingEventFlag) {
    stroke(255,0,0);
    rect(width-10, 0, 10, 10);
    drawMetro();
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

void drawMetro() {
  float t = 60*1000*3/(bpm*50);
  if((playback_clock%(int)t) < 16) {
    fill(255,0,0, (16-(playback_clock%(int)t))*255/16);
    ellipse(width/2, height/2, 200,200);
    noFill();
  }
}