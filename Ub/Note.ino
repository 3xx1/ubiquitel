void setLoop() {
  looptime = packet[1]/res;

  for(int i=0;i<numNotes;i++) {
    if(notes[i].sp<0) notes[i].sp += looptime;
  }
}

void setNote() {
  numNotes = (packetSize-1)/8;
  
  for(int i=0;i<numNotes;i++) {
    notes[i].ts = (int)packet[2*i+1]/res;
    notes[i].v = (int)packet[2*i+2]*4;
    notes[i].sp = notes[i].ts -notes[i].v;
    if(notes[i].sp<0) notes[i].sp += looptime;
  }

}

void resetNote() {
  if(numNotes) {
    for(int i=0;i<numNotes;i++) {
      notes[i].ts = 0;
      notes[i].v = 0;
      notes[i].sp = 0;
    }
  numNotes = 0;
  }
}

