void notifyUb() {
  ubmip = udp.remoteIP();
  sendData(UB_FOUND);
  if(dockState == LOW)
    sendData(UB_DOCKED);
}

void syncUb() {
  gtime = 0;
  playtime = 100;
  stopUb();
}

void playUb() {
  if(numTaps[r] == 0) {
    isPlaying = false;
    if(numTaps[(r+1)%2]) {
      r = (r+1)%2;
      resetTaps();
      w = r;
    }
    return;
    }
  if(!isPlaying) {
    now = looptime[r]-40;
    rcnt = repeat[r];
    //次のバッファにデータがないとき
    if(numTaps[(r+1)%2]==0) sendData(UB_PLAYED);
    //タイマー再生機能
    //if(packet[1]/res>gtime) playtime = (int)packet[1]/res;
    //else isPlaying =true;
    isPlaying = true;
      Serial.println("ts");
    for(int i=0;i<numTaps[r];i++) {
      Serial.println(taps[r][i].sp);
    }
  }
  Serial.println("play UB!");
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
    if(taps[r][next].sp == now && numTaps[r] > 0){
      tapping = true;
            Serial.println("gtimePrepare");
            Serial.println(gtime);
    }
    if(tapping) {
        stepMotor();
        stepCount++;
        if(stepCount == taps[r][next].v) {
            Serial.println("gtimeRelease");
            Serial.println(gtime);

            tapping = false;
            stepCount = 0;
            stopMotor();
            if(next < numTaps[r]-1) next++;
            else {
              next = 0;
              if(rcnt == 1) {
                if(numTaps[(r+1)%2]>0) {//次のバッファにデータがあるとき
                  rcnt = repeat[(r+1)%2];//リピート回数設定
                  now = now + looptime[(r+1)%2] - looptime[r];//再生位置移動
                  resetTaps();
                  sendData(UB_PLAYED);                 
                }
                else {
                  stopUb();//次のバッファにデータがなければ停止
                  sendData(UB_STOPPED);
                }
              }
              else if(rcnt == 0 && numTaps[(r+1)%2]>0) {
                rcnt = repeat[(r+1)%2];//リピート回数設定
                now = now + looptime[(r+1)%2] - looptime[r];//再生位置移動
                resetTaps(); 
              }
              else if(rcnt > 1) rcnt--;
            }
        }
    }
    now = now+1;
    if(now == looptime[r]) {now = 0;Serial.println("now is 0!");Serial.println(looptime[r]);}
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
