#include <RH_ASK.h>
#include <SPI.h>
#define fsrAnalogpin 3
int fsrReading;

RH_ASK driver;

void setup()
{

}

void loop()
{   
    fsrReading = analogRead(fsrAnalogpin);
    int lengthFsr = sizeof(fsrReading/2);
    uint8_t digits[3];
    // Send fsr Reading to 433 mhz receiver
    for (int i = 0; i < 3; i++){
      if ( i < lengthFsr ){
        digits[i] = fsrReading % 10 + 48;
        fsrReading /= 10;
      } else {
        digits[i] = 97;
      };
    };
    driver.send(digits, lengthFsr);
    driver.waitPacketSent();
    
    delay(100);
}
