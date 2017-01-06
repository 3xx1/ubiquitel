void keyPressed() {
  if(key=='r')
  {
    if(recordingEventFlag) {
      recordingEventFlag = false;
      intensity_rec.add(0);
      timestamp_rec.add(clock);
    } else {
      recordingEventFlag = true;
      recordingInit();
    }

    stroke(255,0,0);
    rect(width-10, 0, 10, 10);
  }

  if (key == 'p') {
    if(playbackEventFlag) {
      playbackEventFlag = false;
      println("rstop,");
    } else {
      println("rstart,");
      playbackEventFlag = true;
      playbackInit();
    }
  }
  if (key == 'n') {
    println("stop,");
  }
  if (key == 'x') {
    if(repeatEventFlag) {
      repeatEventFlag = false;
    } else {
      repeatEventFlag = true;
    }
  }
  if (key == 'a') {
    println("sync,");
  }
  if (key == 'z') {
    println("allstop,");
  }
}