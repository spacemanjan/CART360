#include "SD.h"
#define SD_ChipSelectPin 10
#include "TMRpcm.h"
#include "SPI.h"

TMRpcm tmrpcm;
int controllerPin = 7;

void setup(){
pinMode(controllerPin, INPUT); 
tmrpcm.speakerPin = 9;
tmrpcm.setVolume(6);
tmrpcm.play("rain.wav");
}

void loop(){  
  if (digitalRead(controllerPin) == 1){
      tmrpcm.pause(); 
  } else {
    if (tmrpcm.isPlaying() == 0) {
      tmrpcm.pause(); 
    }
  }
}
