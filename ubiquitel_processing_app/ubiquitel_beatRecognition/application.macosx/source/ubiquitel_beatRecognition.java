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

float bpm = 120;
boolean quantize = true;
int  clk_last_note = 0;

// data storage
ArrayList<Integer> intensity_rec = new ArrayList<Integer>();
ArrayList<Integer> timestamp_rec = new ArrayList<Integer>();

String systemMessage = "press 'r' to start recording, press 'p' to play recorded stuff, press 'x' to repeat it.";

public void setup() {
  
  intensityRecognitionEventFlag = false;
  recordingEventFlag = true;
  playbackEventFlag = false;
  println(systemMessage);
  myPort = new Serial(this, "/dev/cu.usbmodemFD141", 9600);
}

public void draw() {
  background(255);
  clock++;
  playback_clock++;
  /* ******************************************************************* */
  /* take the second line out, switch first line to arduino analog input */
  if (myPort.available()>0) signal[0] = myPort.read();
  //signal[0] = pulseInput;
  pulseInput = pulseReductionSimulator(pulseInput, .8f);
  /* ******************************************************************* */

  drawPeripherals();
  if(playbackEventFlag)  playbackRecordings(playback_clock, playback_index);
  if(!playbackEventFlag) intensityRecognitionEvent(signal, threshold_on, threshold_off);
  pushArrayData(signal, arraySize);
  osciloLineDraw(signal, arraySize);
}
// to be removed.
public void drawPeripherals() {
  if (recordingEventFlag) {
    stroke(255,0,0);
    rect(width-10, 0, 10, 10);
    drawMetro();
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
  for(int i=0; i<arraySize-1; i++) {
    line(i, height - signal[i], i+1, height - signal[i+1]);
  }
}

public void drawMetro() {
  float t = 60*1000*3/(bpm*50);
  if((playback_clock%(int)t) < t/2) {
    noStroke();
    fill(255,0,0, ((t/2)-(playback_clock%(int)t))*255/(t/2));
    ellipse(width/2, height/2, 200,200);
    noFill();
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
      if(!quantize)
        println("add," + peakTiming + "," + maxIntensity);
      else {
        int qpt = (int)((peakTiming+8)/15) * 15;
        clk_last_note = qpt;
        println("record," + maxIntensity);
      }
    }
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
      //println("sendNotes,");
    } else {
      recordingEventFlag = true;
      recordingInit();
      println("reset,");
      if(!quantize);
        //println("addloop,"+clock+0);
      else {
        float t = 60*1000*3/(bpm*50);
        int qclk = ((int)(clk_last_note/(t*4))+1)*(int)t*4; 
        //println("addloop," + 256 + "," + 0);
      }
    }

    stroke(255,0,0);
    rect(width-10, 0, 10, 10);
  }

  if (key == 'p') {
    if(playbackEventFlag) {
      playbackEventFlag = false;
      println("play,");
    } else {
      playbackEventFlag = true;
      playbackInit();
      println("play,Rhythm,");
    }
  }
  
  if (key == 'x') {
    if(repeatEventFlag) {
      repeatEventFlag = false;
    } else {
      repeatEventFlag = true;
    }
  }
  if (key == 's') {
      println("stop,");
  }
    if (key == 'f') {
      println("search,");
  }
  if (key == 'y') {
      println("sync,");
  }
    if (key == 'h') {
      println("allstop,");
  }
  if (key == 'q') {
      quantize = !quantize;
  }
  if (key == '1') {
    println("reset,");
    println("add," + 0 + "," + 8*20);
    println("add," + 15*4 + "," + 8*20);
    println("add," + 15*7 + "," + 8*20);
    println("addLoop,"+15*8 + "," + 0);
    println("sendNotes,");
  }
  if (key == '2') {
    println("reset,");
    println("add," + 15*0 + "," + 8*20);
    println("add," + 15*4 + "," + 8*20);
    println("add," + 15*5 + "," + 8*20);
    println("addLoop,"+15*8 + "," + 1);
    println("sendNotes,");
  }
  if (key == '3') {
    println("reset,");
    println("add," + 0 + "," + 8*20);
    println("add," + 15*1 + "," + 8*20);
    println("add," + 15*2 + "," + 8*20);
    println("add," + 15*3 + "," + 8*20);
    println("add," + 15*4 + "," + 8*20);
    println("add," + 15*5 + "," + 8*20);
    println("add," + 15*6 + "," + 8*20);
    println("add," + 15*7 + "," + 8*20);
    println("addLoop,"+15*8 + "," + 2);
    println("sendNotes,");
  }
  if (key == '4') {
    println("reset,");
    println("add," + 24*0 + "," + 5*20);
    println("add," + 24*1 + "," + 5*20);
    println("add," + 24*2 + "," + 5*20);
    println("add," + 24*3 + "," + 5*20);
    println("add," + 24*4 + "," + 5*20);
    println("addLoop,"+15*8 + "," + 3);
    println("sendNotes,");
  }
  if (key == '5') {
    println("reset,");
    println("add," + 0 + "," + 3*20);
    println("add," + 15*2 + "," + 3*20);
    println("add," + 15*5 + "," + 3*20);
    println("addLoop,"+15*8 + "," + 4);
    println("sendNotes,");
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
      playbackEventFlag = false;
      println("playback done");
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
