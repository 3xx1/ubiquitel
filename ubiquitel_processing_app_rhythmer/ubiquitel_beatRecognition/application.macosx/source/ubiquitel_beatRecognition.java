import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import processing.serial.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class ubiquitel_beatRecognition extends PApplet {

// Comment Out If Not Using Arduino

Serial myPort;

// define array size here
static int arraySize = 800;

// array for input signal storage
int signal[] = new int[arraySize];

// threshold values for intensityRecognitionEvent
int threshold_on  = 30;
int threshold_off = 30;

// global valuables
int pulseInput;
int maxIntensity;
int peakTiming;
boolean intensityRecognitionEventFlag;
boolean recordingEventFlag;
boolean playbackEventFlag;
boolean repeatEventFlag;
int clock;
int playback_clock;
int playback_index;

// data storage
ArrayList<Integer> intensity_rec = new ArrayList<Integer>();
ArrayList<Integer> timestamp_rec = new ArrayList<Integer>();

String systemMessage = "press 'r' to start recording, press 'p' to play recorded stuff, press 'x' to repeat it.";

public void setup() {
  
  intensityRecognitionEventFlag = false;
  recordingEventFlag = false;
  playbackEventFlag = false;
  println(systemMessage);
  innerWidth = width - 2*padding;
  myPort = new Serial(this, "/dev/cu.usbmodem1411", 9600);
}

public void draw() {
  background(255);
  //grid();
  //rhythm();
  //if(playbackEventFlag) highlight();
  clock++;
  playback_clock++;
  /* ******************************************************************* */
  /* take the second line out, switch first line to arduino analog input */
  if (myPort.available()>0) signal[0] = myPort.read();
  // signal[0] = pulseInput;
  //signal[0] = highlight() * 100;
  pulseInput = pulseReductionSimulator(pulseInput, .8f);
  /* ******************************************************************* */

  // drawPeripherals();
  //if(playbackEventFlag)  playbackRecordings(playback_clock, playback_index);
  if(playbackEventFlag) intensityRecognitionEvent(signal, threshold_on, threshold_off);
  pushArrayData(signal, arraySize);
  osciloLineDraw(signal, arraySize);
}
// to be removed.
public void drawPeripherals() {
  if (recordingEventFlag) {
    stroke(255,0,0);
    rect(width-10, 0, 10, 10);
  }
  
  if (playbackEventFlag) {
    stroke(0,255,0);
    rect(width-20, 0, 10, 10);
  }
}

public void pushArrayData(int[] signal, int arraySize) {
  for(int i=1; i<arraySize; i++) {
    signal[arraySize-i] = signal[arraySize-i-1];
  }
}

public void osciloLineDraw(int[] signal, int arraySize) {
  stroke(0);
  strokeWeight(1);
  for(int i=0; i<arraySize-1; i++) {
    line(i, height - signal[i], i+1, height - signal[i+1]);
  }
}
// not for actual future dev use, just faking input pulse :)
// comment all below out

public void mousePressed() {
  pulseInput = fakeIntensityReturn(mouseY) + PApplet.parseInt(random(-10, 10));
}

public int fakeIntensityReturn(int y) {
  int r = PApplet.parseInt( map(y, height, 0, 0, 255) );
  return r;
}

public int pulseReductionSimulator(int intensity, float reduction) {
  int r = PApplet.parseInt(intensity*reduction);
  if(r<1) {
    return 0;
  } else {
    return r;
  }
}
// to be removed.
public void intensityRecognitionEvent(int[] signal, int threshold_on, int threshold_off) {
  // TODO: to be cleaned up
  if(signal[0]>threshold_on && !intensityRecognitionEventFlag) {
    intensityRecognitionEventFlag = true;
    maxIntensity = 0;
  }

  if(intensityRecognitionEventFlag) {
    if(maxIntensity<signal[0]) {
      maxIntensity = signal[0];
      peakTiming = clock;
    }
  }

  if(signal[0]<threshold_off && intensityRecognitionEventFlag) {
    intensityRecognitionEventFlag = false;
    if (recordingEventFlag) {
      intensity_rec.add(maxIntensity);
      if(timestamp_rec.size()<1) {
        clock = 0;
        peakTiming = 0;
      }
      timestamp_rec.add(peakTiming);
    }
    println("record," + maxIntensity);
  }

  if(intensityRecognitionEventFlag) rect(0, 0, 50, 50);
}
public void keyPressed() {
  if(key=='r')
  {
    if(recordingEventFlag) {
      recordingEventFlag = false;
      intensity_rec.add(0);
      timestamp_rec.add(clock);
    } else {
      recordingEventFlag = true;
      recordingInit();
    }

    stroke(255,0,0);
    rect(width-10, 0, 10, 10);
  }

  if (key == 'p') {
    if(playbackEventFlag) {
      playbackEventFlag = false;
      println("rstop,");
    } else {
      println("rstart,");
      playbackEventFlag = true;
      playbackInit();
    }
  }
  if (key == 'n') {
    println("stop,");
  }
  if (key == 'x') {
    if(repeatEventFlag) {
      repeatEventFlag = false;
    } else {
      repeatEventFlag = true;
    }
  }
  if (key == 'a') {
    println("sync,");
  }
  if (key == 'z') {
    println("allstop,");
  }
}
public float timestamp() {
  return clock/frameRate; // TODO: replace frameCount with actual app clock later.
}
public void playbackInit() {
  playback_clock = -1;
  playback_index = 0;
}

public void playbackRecordings (int playback_clock, int index) {
  // should appear on draw function
  if (index >= timestamp_rec.size()) {
    if (repeatEventFlag) {
      playbackInit();
      return;
    } else {
      //playbackEventFlag = false;
      //println("playback done");
      return;
    }
  }
  if (timestamp_rec.get(playback_index) == playback_clock) {
    signal[0] = intensity_rec.get(playback_index);
    int intensityConverted = PApplet.parseInt(map(signal[0], 0, 255, 0, 20));
    myPort.write(intensityConverted);
    playback_index++;
  }
}
public void recordingInit() {
  clock = 0;
  
  if (intensity_rec.size()>0) {
    for (int i = intensity_rec.size() - 1; i >= 0; i--) {
      intensity_rec.remove(i);
    }
  }

  if (timestamp_rec.size()>0) {
    for (int i = timestamp_rec.size() - 1; i >= 0; i--) {
      timestamp_rec.remove(i);
    }
  }
}
int padding = 100;
float innerWidth;
int rhythm[] = {1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0};
int r0[] = {1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0};
int r1[] = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
int r2[] = {1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0};
int r3[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int rID = 0;

public void nextRhythm() {
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

public void grid() {
  stroke(0); 
  noFill();
  strokeWeight(2);
  for (int i=0; i<16; i++) rect(padding+i*innerWidth/16, height/2-innerWidth/32, innerWidth/16, innerWidth/16);
  strokeWeight(5);
  for (int i=0; i<4; i++) rect(padding+i*innerWidth/4, height/2-innerWidth/32, innerWidth/16, innerWidth/16);
}

public void rhythm() {
  noStroke(); 
  fill(239, 69, 74, 80);
  for (int i=0; i<16; i++) {
    if (rhythm[i] > 0) rect(padding+i*innerWidth/16+10, height/2-innerWidth/32+10, innerWidth/16-20, innerWidth/16-20);
  }
}

public int highlight() {
  fill(239, 69, 74);
  int pos = (playback_clock/20)%16;
  rect(padding+pos*innerWidth/16, height/2+innerWidth/16, innerWidth/16, innerWidth/128);
  if (rhythm[pos] > 0) {
    rect(padding+pos*innerWidth/16, height/2-innerWidth/32, innerWidth/16, innerWidth/16);
    if (playback_clock%20<2 && rhythm[pos]>0) return rhythm[pos];
  }
  return 0;
}
  public void settings() {  size(800, 400); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "ubiquitel_beatRecognition" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
