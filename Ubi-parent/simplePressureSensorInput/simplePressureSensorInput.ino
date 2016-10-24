int sensorPin = A0;
unsigned long sensorValue = 0;
unsigned long out[10] = {0,0,0,0,0,0,0,0,0,0};
int cnt = 0;
void setup() {
  Serial.begin(9600);
}

void loop() {
  out[cnt%7] = map(analogRead(sensorPin),0,1023,0,255);
  out[cnt%7] = out[cnt%7]*out[cnt%7]*out[cnt%7]*out[cnt%7]/255/255/255;
  cnt++;

  if(cnt%7 == 0) {
    for(int i=0;i<7;i++) { 
      sensorValue += out[i]/7;
    }
    Serial.write(sensorValue);
    sensorValue = 0;
  }
  delay(5);
}
