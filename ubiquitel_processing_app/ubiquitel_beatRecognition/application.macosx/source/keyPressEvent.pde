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
    if (key == 'h') {
      println("allstop,");
  }
  if (key == 'q') {
      quantize = !quantize;
  }
  if (key == '1') {
    println("reset,");
    println("add," + 0 + "," + 8*20);
    println("add," + 15*4 + "," + 8*20);
    println("add," + 15*7 + "," + 8*20);
    println("sendNotes,");
    println("sendLoop,"+15*8 + "," + 0);
  }
  if (key == '2') {
    println("reset,");
    println("add," + 15*0 + "," + 8*20);
    println("add," + 15*4 + "," + 8*20);
    println("add," + 15*5 + "," + 8*20);
    println("sendNotes,");
    println("sendLoop,"+15*8 + "," + 1);
  }
  if (key == '3') {
    println("reset,");
    println("add," + 0 + "," + 8*20);
    println("add," + 15*1 + "," + 8*20);
    println("add," + 15*2 + "," + 8*20);
    println("add," + 15*3 + "," + 8*20);
    println("add," + 15*4 + "," + 8*20);
    println("add," + 15*5 + "," + 8*20);
    println("add," + 15*6 + "," + 8*20);
    println("add," + 15*7 + "," + 8*20);
    println("sendNotes,");
    println("sendLoop,"+15*8+ "," + 2);
  }
  if (key == '4') {
    println("reset,");
    println("add," + 24*0 + "," + 5*20);
    println("add," + 24*1 + "," + 5*20);
    println("add," + 24*2 + "," + 5*20);
    println("add," + 24*3 + "," + 5*20);
    println("add," + 24*4 + "," + 5*20);
    println("sendNotes,");
    println("sendLoop,"+15*8+ "," + 3);
  }
  if (key == '5') {
    println("reset,");
    println("add," + 0 + "," + 3*20);
    println("add," + 15*2 + "," + 3*20);
    println("add," + 15*5 + "," + 3*20);
    println("sendNotes,");
    println("sendLoop,"+15*8+ "," + 4);
  }
}