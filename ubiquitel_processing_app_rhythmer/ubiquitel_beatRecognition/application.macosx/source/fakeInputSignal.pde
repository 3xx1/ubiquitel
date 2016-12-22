// not for actual future dev use, just faking input pulse :)
// comment all below out

void mousePressed() {
  pulseInput = fakeIntensityReturn(mouseY) + int(random(-10, 10));
}

int fakeIntensityReturn(int y) {
  int r = int( map(y, height, 0, 0, 255) );
  return r;
}

int pulseReductionSimulator(int intensity, float reduction) {
  int r = int(intensity*reduction);
  if(r<1) {
    return 0;
  } else {
    return r;
  }
}