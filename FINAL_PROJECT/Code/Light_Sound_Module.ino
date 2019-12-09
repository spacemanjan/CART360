//Variables, ints, longs, #includes and #defines go here
char digits[3];

// nrf24_server
#include <SPI.h>
#include <RH_NRF24.h>
RH_NRF24 nrf24;

// LED_stuff
#include <FastLED.h>
#define LED_PIN     6
#define NUM_LEDS    250
#define BRIGHTNESS  255
#define FRAMES_PER_SECOND  120
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGBArray<NUM_LEDS> leds;
#define UPDATES_PER_SECOND 100


int FADE_TEMPO = 0;
// Music_stuff  
#define MUSIC_PIN   8
#define SPEAKER_PIN 3

void setup() {
  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
}

extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
extern const uint8_t gGradientPaletteCount;
                               // Current palette number from the 'playlist' of color palettes
uint8_t gCurrentPaletteNumber;

CRGBPalette16 gCurrentPalette( CRGB::Black);
CRGBPalette16 gTargetPalette( gGradientPalettes[0] );

void loop() {
  deskbuddyread();
  squishRead();
  colorwaves(leds, NUM_LEDS, gCurrentPalette);
  LEDctrl();
  music();
}

void deskbuddyread() {
  if (nrf24.available())
  {
    // Should be a message for us now   
    uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (nrf24.recv(buf, &len))
    {
      String result = "";
      for( int i=0; i<3; i++){
        result += char(buf[i]);
      }
    result.toCharArray(digits, 3);
  }
 }
}

void squishRead() {
   if (digits[2] == 1){
      FADE_TEMPO = 20;
   } else if (digits[2] == 2){
      FADE_TEMPO = 40;
   } else if (digits[2] == 3){
      FADE_TEMPO = 60;
   } else if (digits[2] == 4){
      FADE_TEMPO = 80;
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

void LEDctrl(){

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

  // do some periodic updates
  EVERY_N_MILLISECONDS( 8000 ) { fade(); } // slowly cycle the "base color" through the rainbow
  if (digits[0] == 1){
     gCurrentPaletteNumber = 1;
  } else if (digits[0] == 2){
    gCurrentPaletteNumber = 2;
  } else if (digits[0] == 3){
    gCurrentPaletteNumber = 3;
  } else if (digits[0] == 4){
    gCurrentPaletteNumber = 4;
  }
}

void fade(){
  leds.fadeToBlackBy(FADE_TEMPO);
}

void music(){
  if (digits[1] == 2){
    digitalWrite(SPEAKER_PIN, LOW);
  } else if (digits[1] == 3){
    digitalWrite(SPEAKER_PIN,HIGH);
  }
}

DEFINE_GRADIENT_PALETTE( Coral_reef_gp ) {
    0,  40,199,197,
   50,  10,152,155,
   96,   1,111,120,
   96,  43,127,162,
  139,  10, 73,111,
  255,   1, 34, 71};
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
DEFINE_GRADIENT_PALETTE( es_autumn_19_gp ) {
    0,  26,  1,  1,
   51,  67,  4,  1,
   84, 118, 14,  1,
  104, 137,152, 52,
  112, 113, 65,  1,
  122, 133,149, 59,
  124, 137,152, 52,
  135, 113, 65,  1,
  142, 139,154, 46,
  163, 113, 13,  1,
  204,  55,  3,  1,
  249,  17,  1,  1,
  255,  17,  1,  1};
const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
  Coral_reef_gp,
  es_pinksplash_08_gp,
  es_emerald_dragon_08_gp,
  es_autumn_19_gp };
  
const uint8_t gGradientPaletteCount = 
  sizeof( gGradientPalettes) / sizeof( TProgmemRGBGradientPalettePtr );
