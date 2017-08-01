int sensorPin = A0;
unsigned char sensorValue = 0;
unsigned long out[10] = {0,0,0,0,0,0,0,0,0,0};
int cnt = 0;
void setup() {
  Serial.begin(9600);
}

void loop() {
  double tmp = map(analogRead(sensorPin),0,1023,0,255)/255.0;
  for(int i=0;i<2;i++)
    tmp *= tmp;
  out[cnt%10] = tmp*255;
  cnt++;

  if(cnt%10 == 0) {
    for(int i=0;i<10;i++) { 
      sensorValue += out[i]/10;
    }
    Serial.write(sensorValue);
    sensorValue = 0;
  }
  delay(4);
}
