void recordingInit() {
  clock = 0;
  
  if (intensity_rec.size()>0) {
    for (int i = intensity_rec.size() - 1; i >= 0; i--) {
      intensity_rec.remove(i);
    }
  }

  if (timestamp_rec.size()>0) {
    for (int i = timestamp_rec.size() - 1; i >= 0; i--) {
      timestamp_rec.remove(i);
    }
  }
}