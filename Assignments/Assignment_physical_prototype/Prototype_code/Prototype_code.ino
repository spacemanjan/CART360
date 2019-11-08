//=====LEDS=====//
#include <FastLED.h>           //Include FastLED library

#define NUM_LEDS 60             //How many LEDS
#define LED_PIN 5              //LED DATA PIN
#define LED_TYPE WS2812        //LED STRIP TYPE
#define COLOR_ORDER GRB        //COLOR TYPE GRB(correct Color order)
#define BRIGHTNESS 240         //Brightness Range [off..on] = [0..255] 

CRGB leds[NUM_LEDS];           //Define the array of RGB control data for each LED              
                               // Forward declarations of an array of cpt-city gradient palettes, and 
                               // a count of how many there are.  The actual color palette definitions
                               // are at the bottom of this file.
#define SECONDS_PER_PALETTE 10
//====LEDS-END===//

//====SENSORS====//
#define fsrAnalogPin A0        //PRESSURE SENSOR PIN
int fsrReading;                //Analog reading from PRESSURE SENSOR
int pressureThreshold = 100;
int smallTrigger = 0;
int smallHits = 0;
int prevFSR = 0;

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.
#define pulseSensorPin A2         //PULSE SENSOR PIN
int pulseReading;              //Analog reading from PULSE SENSOR
int pulseThreshold = 550;
int heartBeat = 0;
PulseSensorPlayground pulseSensor;
//==SENSORS-END==//

//FUTURE-PROTOTYPE
//====T&R========//
 #define rfTransmitPin 4  //RF Transmitter pin = digital pin 4
 #define rfReceivePin A1
//====T&R-END====//


//======TIME=====//
unsigned long timeValueA;      //Timer to observe pressure Sensor over time
unsigned long prevTimeValue;    

unsigned long timeValueB;      //PulseSensor Timer
unsigned long prevTimeValueB; 
//===TIME-END===//

void setup() {
// Begin running Serial console/communication with Arduino
  Serial.begin(9600);

// Initialize library using CRGB LED array
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS)
  //cpt-city palettes have different color balance
         .setDither(BRIGHTNESS < 255);
  //set mast brightness control
  FastLED.setBrightness(BRIGHTNESS);

  //PulseSensor SETUP
  pulseSensor.analogInput(pulseSensorPin);
  pulseSensor.setThreshold(pulseThreshold);
  
  //FUTURE-PROTOTYPE
  //Transmitter SETUP
  pinMode(rfTransmitPin, OUTPUT);     
}


//=====LED PALETTE====//
extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
extern const uint8_t gGradientPaletteCount;
                               // Current palette number from the 'playlist' of color palettes
uint8_t gCurrentPaletteNumber;

CRGBPalette16 gCurrentPalette( CRGB::Black);
CRGBPalette16 gTargetPalette( gGradientPalettes[0] );
//=====LED PALETTE====//


void loop() {
  colorwaves(leds, NUM_LEDS, gCurrentPalette);    //loop which moves the leds in waves
  pressureWatch();                                //fsr pressure sensor loop
  paletteControl();                               //LED_loop
  serialReadings();                               //All Serial.println go here
  pulseWatch();                                   //Pulse sensor loop *sensor doesn't seem to work*
//  transmit();                                   //Transmitter Code
  FastLED.show();                                 //show change in LEDS
  timeValueA = millis();                          //time-a value start
  timeValueB = millis();                          //time-b value start
}



//==SENSOR-CODE-BITS==//
void pressureWatch() {
  //fsrReading is the int which stores the analog data from fsrPin
  fsrReading = analogRead(fsrAnalogPin);
  //Color limit 10seconds
  int interval = 10000;

  //if 10 seconds have elapsed...
  //store the number of fsr over threshold readings (smallTrigger) in smallHits
  //reset smallTrigger to 0
  if (timeValueA - prevTimeValue >= interval){
    smallHits = smallTrigger;
    prevTimeValue = timeValueA;
    smallTrigger = 0;
  //if 10 seconds have not elapsed...
  //check for fsrReadings which are over the Threshold
  //add 1 to smallTrigger everytime 
  }else{
    if (fsrReading > pressureThreshold){
        if (prevFSR < pressureThreshold){
          smallTrigger += 1;
        }
    }
   prevFSR = fsrReading;
  }

  //LED_CONTROL BASED ON FSR OUTPUT IN Prev 10 seconds
  if (smallHits == 0){
    gCurrentPaletteNumber = 4;
  }else if (smallHits >= 1 && smallHits < 5){
    gCurrentPaletteNumber = 3;
  }else if (smallHits >= 5 && smallHits < 10){
    gCurrentPaletteNumber = 5;
  } else if (smallHits >= 10 && smallHits < 15){
    gCurrentPaletteNumber = 2;
  } else {
    gCurrentPaletteNumber = 3;
  }
}


//UNDER CONSTRUCTION/INVESTIGATION
//try as I might I can't seem to get my pulse sensor to notice a heartbeat
//broken sensor? thick skin? bad code? maybe all 3
void pulseWatch() {
//  int myBPM = pulseSensor.getBeatsPerMinute();
 //Gives me values of 895-909
  int myBPM = analogRead(pulseSensorPin);
  int bpmTime;
  //time interval
  int interval = 10000;
//If the pulse sensor is touching skin(thats what I'm getting here actually)
  if (timeValueB - prevTimeValueB >= interval){
    bpmTime = heartBeat;
    prevTimeValueB = timeValueB;
    heartBeat = 0;
  } else {
    if (myBPM > 899){
     heartBeat += 1;
    }
  }
}


void serialReadings(){
//  Serial.println("Pressure reading = ");
//  Serial.println(fsrReading);
//  Serial.println(gCurrentPaletteNumber);
//  Serial.println(smallTrigger);
//  Serial.println(smallHits);
}


//===LED-CODE-BITS===//
void paletteControl() {
  
  gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];

  EVERY_N_MILLISECONDS(40) {
    nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 16);
  }
}



void colorwaves( CRGB* ledarray, uint16_t numleds, CRGBPalette16& palette) 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 300, 1500);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < numleds; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    uint8_t index = hue8;
    //index = triwave8( index);
    index = scale8( index, 240);

    CRGB newcolor = ColorFromPalette( palette, index, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (numleds-1) - pixelnumber;
    
    nblend( ledarray[pixelnumber], newcolor, 128);
  }
}


//void practiceLEDS() {
//////Using an array to fill all LEDS one color
//  for (int i = 0; i < NUM_LEDS; i++){
//      leds[i] = CRGB(255,255,255);
//  }
//////Configure the RGB value of a selected LED
//  leds[0] = CRGB(255,255,255);
////// use a routine to fill a block of LEDS with a fixed color
//////number of LEDS from the first fixed LED[X] that I want filled//
//  fill_solid( &(leds[0]),5, CRGB(100,20,130));
//////CHSV configures LEDS using HUE,SATURATION,BRIGHTNESS
//  leds[4] = CHSV( HUE_PURPLE, 150, 255);
//////configure LEDS using web color code name
//  leds[5] = CRGB::HotPink;
////Send a bolt of 3 red leds and have them turn black behind them 
//   for (int i = 0; i < NUM_LEDS; i++){
//      leds[i] = CRGB(0,0,0);
//      leds[i+3] = CRGB(255,0,0);
//      FastLED.show();   //activate all changes to LEDS
//      delay(100);
//    }
//}

// Gradient palette "es_rivendell_15_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/rivendell/tn/es_rivendell_15.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_rivendell_15_gp ) {
    0,   1, 14,  5,
  101,  16, 36, 14,
  165,  56, 68, 30,
  242, 150,156, 99,
  255, 150,156, 99};


// Gradient palette "Coral_reef_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/other/tn/Coral_reef.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE( Coral_reef_gp ) {
    0,  40,199,197,
   50,  10,152,155,
   96,   1,111,120,
   96,  43,127,162,
  139,  10, 73,111,
  255,   1, 34, 71};

// Gradient palette "es_ocean_breeze_068_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/ocean_breeze/tn/es_ocean_breeze_068.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 24 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_ocean_breeze_068_gp ) {
    0, 100,156,153,
   51,   1, 99,137,
  101,   1, 68, 84,
  104,  35,142,168,
  178,   0, 63,117,
  255,   1, 10, 10};

// Gradient palette "es_ocean_breeze_036_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/ocean_breeze/tn/es_ocean_breeze_036.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_ocean_breeze_036_gp ) {
    0,   1,  6,  7,
   89,   1, 99,111,
  153, 144,209,255,
  255,   0, 73, 82};


DEFINE_GRADIENT_PALETTE( es_emerald_dragon_08_gp ) {
    0,  97,255,  1,
  101,  47,133,  1,
  178,  13, 43,  1,
  255,   2, 10,  1};

  DEFINE_GRADIENT_PALETTE( es_pinksplash_08_gp ) {
    0, 126, 11,255,
  127, 197,  1, 22,
  175, 210,157,172,
  221, 157,  3,112,
  255, 157,  3,112};
  DEFINE_GRADIENT_PALETTE( Analogous_1_gp ) {
    0,   3,  0,255,
   63,  23,  0,255,
  127,  67,  0,255,
  191, 142,  0, 45,
  255, 255,  0,  0};
  
// Single array of defined cpt-city color palettes.
// This will let us programmatically choose one based on
// a number, rather than having to activate each explicitly 
// by name every time.
// Since it is const, this array could also be moved 
// into PROGMEM to save SRAM, but for simplicity of illustration
// we'll keep it in a regular SRAM array.
//
// This list of color palettes acts as a "playlist"; you can
// add or delete, or re-arrange as you wish.
const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
  es_rivendell_15_gp,
  es_ocean_breeze_036_gp,
  Coral_reef_gp,
  es_ocean_breeze_068_gp,
  es_pinksplash_08_gp,
  Analogous_1_gp,
  es_emerald_dragon_08_gp};

// Count of how many cpt-city gradients are defined:
const uint8_t gGradientPaletteCount = 
  sizeof( gGradientPalettes) / sizeof( TProgmemRGBGradientPalettePtr );
