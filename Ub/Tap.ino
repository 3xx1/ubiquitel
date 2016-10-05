void notifyUb() {
  ubmip = udp.remoteIP();
  sendData(UB_FOUND);
  if(dockState == LOW)
    sendData(UB_DOCKED);
}

void syncUb() {
  gtime = 0;
  playtime = 20;
  stopUb();
}

void playUb() {
  if(numNotes == 0) isPlaying = false;
  if(!isPlaying) {
    for(int i=0;i<numNotes;i++) {
      if(notes[i].sp<0) notes[i].sp += looptime;
    }
    now = looptime-40;
    Serial.println(gtime);
    if(packet[1]>gtime) playtime = (int)packet[1]/res;
    else isPlaying =true;
  }
}

void pauseUb() {
  if(isPlaying) isPlaying =false;
}

void stopUb() {
  if(isPlaying) isPlaying =false;
  stopMotor();
  next = 0;
  tapping = false;
  stepCount = 0;
}

void stepTime() {
    if(notes[next].sp == now && numNotes > 0)
      tapping = true;
    
    if(tapping) {
        stepMotor();
        stepCount++;
        if(stepCount == notes[next].v) {
            tapping = false;
            stepCount = 0;
            stopMotor();
            next = (next < numNotes-1) ? next+1 : 0;
        }
    }
    now = (now+1)%looptime;
}

void stepMotor()
{
    int thisStep = stepCount%4;
    digitalWrite(Vs2B, HIGH);
    digitalWrite(PS, LOW);
    digitalWrite(LED, HIGH);
  
    switch (thisStep) {
        case 0:    // 00
            digitalWrite(inA, LOW);
            digitalWrite(inB, LOW);
        break;
        case 1:    // 10
            digitalWrite(inA, HIGH);
            digitalWrite(inB, LOW);
        break;
        case 2:    //11
            digitalWrite(inA, HIGH);
            digitalWrite(inB, HIGH);
        break;
        case 3:    //01
        digitalWrite(inA, LOW);
        digitalWrite(inB, HIGH);
        break;
    }
}

void stopMotor() {
    digitalWrite(PS, HIGH);
    digitalWrite(Vs2B, LOW);
    digitalWrite(LED, LOW);
}
