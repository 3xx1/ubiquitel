void intensityRecognitionEvent(int[] signal, int threshold_on, int threshold_off) {
  // TODO: to be cleaned up
  if(signal[0]>threshold_on && !intensityRecognitionEventFlag) {
    intensityRecognitionEventFlag = true;
    maxIntensity = 0;
  }
  
  if(intensityRecognitionEventFlag) {
    if(maxIntensity<signal[0]) maxIntensity = signal[0];
  }
  
  if(signal[0]<threshold_off && intensityRecognitionEventFlag) {
    intensityRecognitionEventFlag = false;
    println(maxIntensity, clock);
    if (recordingEventFlag) {
      intensity_rec.add(maxIntensity);
      timestamp_rec.add(clock);
    }
  }
  
  if(intensityRecognitionEventFlag) rect(0, 0, 50, 50);
}

