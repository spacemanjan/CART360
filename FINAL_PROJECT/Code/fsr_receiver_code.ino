#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile
#define FSR_OUTPUT_PIN A0
RH_ASK driver;

void setup()
{
  
}

void loop()
{
    uint8_t buf[3];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      String str((char*)buf);
      str.toInt();       
    }
    if (str < 200){
      analogWrite(FSR_OUTPUT_PIN, 0);
    } else if (str > 200 && str < 500){
      analogWrite(FSR_OUTPUT_PIN, 1);
    } else if (str > 500  && str < 700){
      analogWrite(FSR_OUTPUT_PIN, 2);
    } else if (str > 700 && str < 900){
      analogWrite(FSR_OUTPUT_PIN, 3);
    }
}
