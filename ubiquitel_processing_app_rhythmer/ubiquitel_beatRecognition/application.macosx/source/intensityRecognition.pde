void intensityRecognitionEvent(int[] signal, int threshold_on, int threshold_off) {
  // TODO: to be cleaned up
  if(signal[0]>threshold_on && !intensityRecognitionEventFlag) {
    intensityRecognitionEventFlag = true;
    maxIntensity = 0;
  }

  if(intensityRecognitionEventFlag) {
    if(maxIntensity<signal[0]) {
      maxIntensity = signal[0];
      peakTiming = clock;
    }
  }

  if(signal[0]<threshold_off && intensityRecognitionEventFlag) {
    intensityRecognitionEventFlag = false;
    if (recordingEventFlag) {
      intensity_rec.add(maxIntensity);
      if(timestamp_rec.size()<1) {
        clock = 0;
        peakTiming = 0;
      }
      timestamp_rec.add(peakTiming);
    }
    println("record," + maxIntensity);
  }

  if(intensityRecognitionEventFlag) rect(0, 0, 50, 50);
}