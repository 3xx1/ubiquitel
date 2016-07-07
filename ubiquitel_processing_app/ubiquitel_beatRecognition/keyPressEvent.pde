void keyPressed() {
  if(key=='r')
  {
    if(recordingEventFlag) {
      recordingEventFlag = false;
      intensity_rec.add(0);
      timestamp_rec.add(clock);
      println("sendNotes,");
      
      if(!quantize)
        println("sendLoop,"+clock);
      else {
        float t = 60*1000*3/(bpm*50);
        int qclk = ((int)(clk_last_note/(t*4))+1)*(int)t*4; 
        println("sendLoop,"+qclk);
      }
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
  if (key == 'q') {
      quantize = !quantize;
  }
}