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
      println("allstop,");
    } else {
      println("play,Demo" + rID%4);
      playbackEventFlag = true;
      playbackInit();
      delay(670);//to sync with Ub
    }
  }
  if (key == 'n') {
    nextRhythm();
    println("recordUbID," + (3-rID%4));
  }
  if (key == 'x') {
    if(repeatEventFlag) {
      repeatEventFlag = false;
    } else {
      repeatEventFlag = true;
    }
  }
}