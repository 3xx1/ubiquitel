PImage finger_up, finger_middle, finger_down;

finger[] fingers;
int fingers_total = 7;

void setup() {
  size(1280, 720);
  finger_up = loadImage("finger_up.png");
  finger_middle = loadImage("finger_middle.png");
  finger_down = loadImage("finger_down.png");
  
  fingers = new finger[fingers_total];
  
  for (int i=0; i<fingers_total; i++) {
    fingers[i] = new finger(0, i*100);
  }
}

void draw() {
  background(0);
  for (int i=0; i<fingers_total; i++) {
    fingers[i].update(mouseY);
    fingers[i].draw();
  }
}

class finger {
  int x, y; 
  int velocity; 
  
  finger(int x_pos, int y_pos) {
    x = x_pos;
    y = y_pos;
    velocity = 0;
  }
  
  void update(int v) {
    velocity = v;
  }
  
  void draw() {
    if (velocity > 63) {
      image(finger_up, x, y, 100, 100);
    } else if (velocity > 31) {
      image(finger_middle, x, y, 100, 100);
    } else {
      image(finger_down, x, y, 100, 100);
    }
  }
}
