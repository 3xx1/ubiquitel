void keyPressed() {
  if(key=='r')
  {
    if(recordingEventFlag) {
      recordingEventFlag = false;
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
    } else {
      playbackEventFlag = true;
      playbackInit();
    }
  }
}


