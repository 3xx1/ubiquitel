void setLoop() {
  looptime[w] = packet[1]/res;
  repeat[w] = packet[2];
  w = (w+1)%2;
  Serial.println("set loop");
}

void setNote() {
  if(numNotes[w] != 0) return;
  numNotes[w] = (packetSize-1)/8;
  for(int i=0;i<numNotes[w];i++) {
    notes[w][i].ts = (int)packet[2*i+1]/res;
    notes[w][i].v = vTable[(int)packet[2*i+2]];
    notes[w][i].sp = notes[w][i].ts -notes[w][i].v;
  }
  Serial.println("set notes");
}

void resetNote() {
  if(numNotes[r]) {
    for(int i=0;i<numNotes[r];i++) {
      notes[r][i].ts = 0;
      notes[r][i].v = 0;
      notes[r][i].sp = 0;
    }
    numNotes[r] = 0;
    looptime[r] = 0;
    Serial.println("reset notes");
    r = (r+1)%2;//バッファ切替
  }
  //next = 0;
}

