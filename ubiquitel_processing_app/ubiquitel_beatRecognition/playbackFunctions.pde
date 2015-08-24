void playbackInit() {
  playback_clock = 0;
  playback_index = 0;
}

void playbackRecordings (int playback_clock, int index) {
  // should appear on draw function
  if (index >= timestamp_rec.size()) {
    playbackEventFlag = false;
    return;
  }
  if (timestamp_rec.get(playback_index) == playback_clock) {
    signal[0] = intensity_rec.get(playback_index);
    playback_index++;
  }
}

