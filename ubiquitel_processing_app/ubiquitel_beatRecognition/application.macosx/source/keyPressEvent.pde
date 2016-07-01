void keyPressed() {
  if(key=='r')
  {
    if(recordingEventFlag) {
      recordingEventFlag = false;
      intensity_rec.add(0);
      timestamp_rec.add(clock);
      println("sendNotes,");
      println("sendLoop,"+clock);
    } else {
      recordingEventFlag = true;
      recordingInit();
      println("reset,");
    }

    stroke(255,0,0);
    rect(width-10, 0, 10, 10);
  }

  if (key == 'p') {
    if(playbackEventFlag) {
      playbackEventFlag = false;
    } else {
      playbackEventFlag = true;
      playbackInit();
      println("play,");
    }
  }
  
  if (key == 'x') {
    if(repeatEventFlag) {
      repeatEventFlag = false;
    } else {
      repeatEventFlag = true;
    }
  }
  if (key == 's') {
      println("stop,");
  }
  if (key == 'y') {
      println("sync,");
  }
}