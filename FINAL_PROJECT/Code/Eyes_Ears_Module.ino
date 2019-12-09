 const unsigned int TRIG_PIN=13;
const unsigned int ECHO_PIN=12;
const unsigned int Distance_thresh=300;
const int Sometime = 200;
#define fsrAnalogPin A1 // FSR is connected to analog 0
int fsrReading;
int mode = 0;
#define MICROPHONE_PIN_OUT A0

const int sampleWindow = 50; // Sample window width in mS (50 mS = 20Hz)
unsigned int sample;
const int maxScale = 8;
const int redZone = 5;

#include <SPI.h>
#include <RH_NRF24.h>

RH_NRF24 nrf24;

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long startMillis= millis();  // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level
 
  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;
  
 //how long did it take to receive the pulse in
 const unsigned long duration= pulseIn(ECHO_PIN, HIGH);
 int distance= duration/29/2;
 uint8_t digits[3];
 
 //SR-O5 DISTANCE SENSOR CODE//
 if(duration==0){
   //SEND MESSAGE SAYING NO ONE IS HERE
   //Means lights off, no sound
   Serial.println("Warning: no pulse from sensor");
   } 
  else{
      if (distance < Distance_thresh/5){
          digits[0] = 1;
      } else if (distance < Distance_thresh/3 && distance > Distance_thresh/5 ){
          digits[0] = 2;
      } else if (distance < Distance_thresh/2 && distance > Distance_thresh/3){
          digits[0] = 3;
      } else if (distance < Distance_thresh && distance > Distance_thresh/2){
          digits[0] = 4;
      }
  }
  //MICROPHONE CODE//
  while (millis() - startMillis < sampleWindow)
   {  
      //take sample from microphone pin
      sample = analogRead(0); 
      if (sample < 1024)  // toss out spurious readings
      {
         if (sample > signalMax)
         {
            signalMax = sample;  // save just the max levels
         }
         else if (sample < signalMin)
         {
            signalMin = sample;  // save just the min levels
         }
      }
   }
   peakToPeak = signalMax - signalMin;
    // map 1v p-p level to the max scale of the display
   int displayPeak = map(peakToPeak, 0, 1023, 0, maxScale);
   // draw the new sample
   for (int i = 0; i <= maxScale; i++)
   {
      if (i >= displayPeak)  // no sound
      {
         digits[1] = 1;
      }
      else if (i < redZone) // normal sound levels 
      {
         digits[1] = 2;
      }
      else // high peaks have been hit 
      {
         digits[1] = 3;
      }
   }
 // FSR CODE //
 fsrReading = analogRead(fsrAnalogPin);
  if (fsrReading > 100 && fsrReading < 200){
    digits[2] = 1;
  } else if (fsrReading > 200 && fsrReading < 300){
    digits[2] = 2;
  } else if (fsrReading > 300 && fsrReading < 400){
    digits[2] = 3;
  } else if (fsrReading > 400 && fsrReading < 500){
    digits[2] = 4;
  }
    
  nrf24.send(digits, sizeof(digits));
  nrf24.waitPacketSent();
  delay(Sometime);
 }
