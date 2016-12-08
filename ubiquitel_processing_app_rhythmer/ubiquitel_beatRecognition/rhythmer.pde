int padding = 100;
float innerWidth;
int rhythm[] = {1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0};

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
  int pos = (frameCount/20)%16;
  rect(padding+pos*innerWidth/16, height/2+innerWidth/16, innerWidth/16, innerWidth/128);
  if (rhythm[pos] > 0) {
    rect(padding+pos*innerWidth/16, height/2-innerWidth/32, innerWidth/16, innerWidth/16);
    if (frameCount%20<2 && rhythm[pos]>0) return rhythm[pos];
  }
  return 0;
}