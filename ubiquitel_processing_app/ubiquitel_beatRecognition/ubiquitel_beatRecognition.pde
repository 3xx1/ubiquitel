void draw() {
  background(255);
  clock++;
  playback_clock++;
  /* ******************************************************************* */
  /* take the second line out, switch first line to arduino analog input */
  // if (myPort.available()>0) signal[0] = myPort.read();
  signal[0] = pulseInput;
  pulseInput = pulseReductionSimulator(pulseInput, .8);
  /* ******************************************************************* */
  
  drawPeripherals();
  if(playbackEventFlag)  playbackRecordings(playback_clock, playback_index);
  if(!playbackEventFlag) intensityRecognitionEvent(signal, threshold_on, threshold_off);
  pushArrayData(signal, arraySize);
  osciloLineDraw(signal, arraySize);
}

