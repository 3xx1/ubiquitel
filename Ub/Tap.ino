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
    Serial.println(rcnt);
    Serial.println(now);
    Serial.println(r);

  if(numNotes[r] == 0) isPlaying = false;
  if(!isPlaying) {
    for(int i=0;i<numNotes[r];i++)
      if(notes[r][i].sp<0) notes[r][i].sp += looptime[r];
    now = looptime[r]-40;
    rcnt = repeat[r];
    Serial.println(gtime);
    Serial.println(rcnt);
    Serial.println(now);
    Serial.println(r);

    if(packet[1]>gtime) playtime = (int)packet[1]/res;
    else isPlaying =true;
  }
}

void pauseUb() {
  if(isPlaying) isPlaying =false;
}

void stopUb() {
  Serial.println("stop");
  if(isPlaying) isPlaying =false;
  stopMotor();
  next = 0;
  tapping = false;
  stepCount = 0;
}

void stepTime() {
    if(notes[r][next].sp == now && numNotes[r] > 0)
      tapping = true;
    
    if(tapping) {
        stepMotor();
        stepCount++;
        if(stepCount == notes[r][next].v) {
            tapping = false;
            stepCount = 0;
            stopMotor();
            if(next < numNotes[r]-1) next++;
            else {
              next = 0;
              if(rcnt == 1) {
                if(numNotes[(r+1)%2]>0) {//次のバッファにデータがあるとき
                  for(int i=0;i<numNotes[(r+1)%2];i++)
                    if(notes[(r+1)%2][i].sp<0) notes[(r+1)%2][i].sp += looptime[(r+1)%2];
                  rcnt = repeat[(r+1)%2];//リピート回数設定
                  now = now + looptime[(r+1)%2] - looptime[r];//再生位置移動
                  resetNote();                  
                }
                else stopUb();//次のバッファにデータがなければ停止
              }
              else if(rcnt > 1) rcnt--;
            }
        }
    }
    now = now+1;
    if(now == looptime[r]) now = 0;
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
