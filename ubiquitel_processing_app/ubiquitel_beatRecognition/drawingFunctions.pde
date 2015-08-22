void pushArrayData(int[] signal, int arraySize) {
  for(int i=1; i<arraySize; i++) {
    signal[arraySize-i] = signal[arraySize-i-1];
  }
}

void osciloLineDraw(int[] signal, int arraySize) {
  for(int i=0; i<arraySize-1; i++) {
    line(i, height - signal[i], i+1, height - signal[i+1]);
  }
}
