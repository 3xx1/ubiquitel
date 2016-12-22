int padding = 100;
float innerWidth;
int rhythm[] = {1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0};
int r0[] = {1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0};
int r1[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
int r2[] = {1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0};
int r3[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int rID = 0;

void nextRhythm() {
  switch(++rID%4) {
    case 0:
      rhythm = r0;
      break;
    case 1:
      rhythm = r1;
      break; 
    case 2:
      rhythm = r2;
      break;
    case 3:
      rhythm = r3;
      break;
    default:
      rhythm = r0;
      break;
  }
}

void grid() {
  stroke(0); 
  noFill();
  strokeWeight(2);
  for (int i=0; i<16; i++) rect(padding+i*innerWidth/16, height/2-innerWidth/32, innerWidth/16, innerWidth/16);
  strokeWeight(5);
  for (int i=0; i<4; i++) rect(padding+i*innerWidth/4, height/2-innerWidth/32, innerWidth/16, innerWidth/16);
}

void rhythm() {
  noStroke(); 
  fill(239, 69, 74, 80);
  for (int i=0; i<16; i++) {
    if (rhythm[i] > 0) rect(padding+i*innerWidth/16+10, height/2-innerWidth/32+10, innerWidth/16-20, innerWidth/16-20);
  }
}

int highlight() {
  fill(239, 69, 74);
  int pos = (playback_clock/20)%16;
  rect(padding+pos*innerWidth/16, height/2+innerWidth/16, innerWidth/16, innerWidth/128);
  if (rhythm[pos] > 0) {
    rect(padding+pos*innerWidth/16, height/2-innerWidth/32, innerWidth/16, innerWidth/16);
    if (playback_clock%20<2 && rhythm[pos]>0) return rhythm[pos];
  }
  return 0;
}