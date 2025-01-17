#include "LedControl.h"

LedControl lc = LedControl(53, 51, 52, 1);

unsigned int sample;
unsigned int ambientSound = 0;

byte rowData[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup(){
  Serial.begin(9600);
  for(int idx = 0; idx < 1; idx++){
    lc.shutdown(idx, false);    
    lc.setIntensity(idx, 1); 
    lc.clearDisplay(idx);  
  }
  ambientSound = measureAmbientSound();
}

void loop(){
  unsigned long startMillis = millis();
  unsigned int peakToPeak = 0;   
  unsigned int signalMax = 0;    
  unsigned int signalMin = 1024;  
  while(millis() - startMillis < 50){
    sample = analogRead(A0);
    if(sample < 1024){     
      if(sample > signalMax){
        signalMax = sample; 
      }else if (sample < signalMin){
        signalMin = sample; 
      }
    }
  }
  peakToPeak = signalMax - signalMin; 
  if(peakToPeak > ambientSound){
      peakToPeak -= ambientSound;
  }else{
      peakToPeak = 0;
  }
  int volume = map(peakToPeak, 0, 1024 - ambientSound, 0, 8); 
  volume = min(volume, 8);
  for(int i = 7; i > 0; i--){
    rowData[i] = rowData[i - 1];
  }
  rowData[0] = 0;
  for(int i = 0; i < volume; i++){
    rowData[0] |= (1 << (7 - i));
  }
  for(int row = 0; row < 8; row++){
    lc.setRow(0, row, rowData[row]);
  }
  delay(25); 
}

unsigned int measureAmbientSound(){
  unsigned long startMillis = millis(); 
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  unsigned int sample;

  while(millis() - startMillis < 500){
    sample = analogRead(A0);
    if(sample < 1024){
      if(sample > signalMax){
        signalMax = sample;
      }else if (sample < signalMin){
        signalMin = sample;
      }
    }
  }
  return signalMax - signalMin;
}
