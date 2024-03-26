

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    15

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 233

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


// setup() function -- runs once at startup --------------------------------

void setup() {


  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(75); // Set BRIGHTNESS to about 1/5 (max = 255)
}


// loop() function -- runs repeatedly as long as board is on ---------------

void loop() {

// colorWipe(strip.Color(127,   127,   127), 100); // White
// colorWipe(strip.Color(0,   0,   0), 100); // Blank
for(int i=1;i<strip.numPixels();i++) {
  bullets(strip.Color(127,   127,   127), i, 10);
}

bullets(strip.Color(127,   127,   127), 8, 15);
bullets(strip.Color(127,   127,   127), 6, 8);
bullets(strip.Color(127,   127,   127), 3, 7);
bullets(strip.Color(127,   127,   127), 3, 6);
bullets(strip.Color(127,   127,   127), 3, 6);
bullets(strip.Color(127,   127,   127), 3, 5);
brightFade(strip.Color(127,   127,   127), 25, 60);
brightFade(strip.Color(127,   127,   127), 7, 100);
brightFade(strip.Color(127,   127,   127), 2, 250);

}


// Some functions of our own for creating animated effects -----------------

// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void bullets(uint32_t color, int bulletSize, int wait) {
  for(int i=0; i<(strip.numPixels()+bulletSize); i++) { // For each pixel in strip...
  for(int j=0; j<bulletSize; j++){
    strip.setPixelColor(i-j, color);         //  Set pixel's color (in RAM)
  }
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
    strip.clear();
  }
}

void brightFade(uint32_t color, int wait, int brightLevel) {
  for(int i=0; i<brightLevel; i++) { // For each pixel in strip...
    strip.setBrightness(i); 
    for(int j=0; j<strip.numPixels(); j++) { // For each pixel in strip...
      strip.setPixelColor(j, color);
    }
  
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

/* 
effectively brightness goes from 0 to 255, but the effective range and visual effect differs, hence the offset
 So what we do is take the incoming numbers, remove the offsets and scale them from there, only re-introducing the brightness offset when returning
LED functional limits to first 2 variables, Tacho range and value to the last 3
Return the actual desired LED brightness
*/

int ledBrightRescale(int ledStripBrightness0pcOffset, int ledStripBrightness100pcOffset, int ledStripLowRescale, int ledStripHiRescale, int ledRescaleValue) {
  int ledStripUsingScale = 255 - ledStripBrightness0pcOffset - ledStripBrightness100pcOffset;

  if (ledRescaleValue < ledStripLowRescale) return (ledStripBrightness0pcOffset);
  if (ledRescaleValue > ledStripHiRescale) return (255-ledStripBrightness100pcOffset);
  return (ledStripBrightness0pcOffset+(ledStripUsingScale*(ledRescaleValue-ledStripLowRescale)/(ledStripHiRescale-ledStripLowRescale)));
}

