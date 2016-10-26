void setLoop() {
  looptime[w] = packet[1]/res;
  repeat[w] = packet[2];
  Serial.println("set loop");
}

void setTaps() {
  if(numTaps[w] != 0) return;
  setLoop();
  numTaps[w] = (packetSize-3*4)/8;
  for(int i=0;i<numTaps[w];i++) {
    taps[w][i].ts = (int)packet[2*i+3]/res;
    taps[w][i].v = vTable[(int)packet[2*i+4]];
    taps[w][i].sp = taps[w][i].ts - taps[w][i].v;
    if(taps[w][i].sp<0) taps[w][i].sp += looptime[w];
    Serial.print(taps[w][i].ts);
    Serial.print(", ");
    Serial.print(taps[w][i].v);
  }
  w = (w+1)%2;
  if(repeat[r] == 0 && isPlaying == true) repeat[r] = 1;
  Serial.println("set taps");
}

void resetTaps() {
  if(numTaps[r]) {
    for(int i=0;i<numTaps[r];i++) {
      taps[r][i].ts = 0;
      taps[r][i].v = 0;
      taps[r][i].sp = 0;
    }
    numTaps[r] = 0;
    looptime[r] = 0;
    r = (r+1)%2;//バッファ切替
    Serial.println("reset taps");
  }
  //next = 0;
}

