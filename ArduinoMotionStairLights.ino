/*
 * Description: Motion activated stair lights.
 * Author: Dean Montgomery
 * Version: 0.6 (Beta)
 * Date: 2015-07-26
 * 
 * 2 PIR sesors at the top and bottom of the stairs.
 * WS28012B Addressable RGB lights - 2 LEDs on each stair - This spread out the strip of 30 and left 2-pairs for spare bulbs.
 * My Arduino is at the top of the stairs and the RGB strip is connected at the top.
 * This will cycle through several varitions of stair walkers.
 * 
*/

#include "FastLED.h"
#include <avr/eeprom.h>

#define NUM_LEDS 26
#define LEDS_PER_STAIR 2        // Number of Leds per stair.  Not yet currenlty changable - just noteable
#define BRIGHTNESS  120         // 0...255  ( used in fade7 )
#define PIN_LED 3               // LED Data pin
#define PIN_PIR_DOWN 5          // PIR Downstairs Pin
#define PIN_PIR_UP 7            // PIR Upstairs Pin
#define GO_UP -1                // Direction control - Arduino at top of stairs
#define GO_DOWN 1               // Direction control - Arduino at top of stairs
int8_t  gLightsOn = LOW;         // track lights on/off
uint8_t gHue = 0;               // track color shifts.
int8_t  gStair = 0;             // track curent stair.
uint8_t gBright = 0;            // track brightness
uint8_t gUpDown[NUM_LEDS];      // directional array to walk/loop up or down stairs.
CRGB    leds[NUM_LEDS];         // setup leds object to access the string
CRGBPalette16 gPalette;         // some favorite and random colors for display.
int8_t  gLastPalette = 15;      // track last chosen palette.
uint8_t gLastWalk = 1;

void setup() {
  delay (3000); // Power Up 3 second safety delay.
  //Serial.begin(9600);
  randomSeed(analogRead(0));
  FastLED.addLeds<WS2812B, PIN_LED, GRB>(leds, NUM_LEDS);  // NOTE set LED string type here.
  pinMode(PIN_PIR_DOWN, INPUT);
  pinMode(PIN_PIR_UP, INPUT);
  digitalWrite(PIN_PIR_DOWN, LOW);
  digitalWrite(PIN_PIR_UP, LOW);
  welcomeRainbow();             // rainbow - give time for PIR sensors to colibrate.
  setUpDown(GO_DOWN);           // populate the array index used for stair direction.
  setPalette();                 // setup some favorite & random colors
}

// Main Loop track PIR sensors.
void loop() {
  // Walk Down.
  if ( digitalRead(PIN_PIR_UP) == HIGH ){
    if ( gLightsOn == LOW ) {
      gLightsOn = HIGH;
      setUpDown(GO_DOWN);
      walker();
      delay(50);
    }
  }
  // Walk Up.
  if ( digitalRead(PIN_PIR_DOWN) == HIGH  ){
    if ( gLightsOn == LOW ) {
      gLightsOn = HIGH;
      setUpDown(GO_UP);
      walker();
      delay(50);
    }
  }
}

// Cycle through stair walkers.
void walker(){
  static uint8_t r = 0;
  r = random8(1, 255); r = random8(1, 255);  // Call random twice - as it wasn't very random.
  if ( r >= 0 && r <= 100 ){
    walk(); // My favorite with random variations.
  } else if ( r > 100 && r <= 175 ){
    flicker(); // Candle with embers.
  } else {
    fade7();
  }
}

// setup walking gUpDown array in forward: 0,1,2,3... or reverse:  ...3,2,1,0
void setUpDown(int8_t upDownDir){
  uint8_t gStairStart = 0;
  if (upDownDir == GO_UP){
    for ( gStair = NUM_LEDS -1; gStair >= 0; gStair-- ){
      gUpDown[gStair] = gStairStart++;
    }
  } else {
    for ( gStair = 0; gStair <= NUM_LEDS; gStair++ ){
      gUpDown[gStair] = gStairStart++;
    }  
  }
}

// Increment to the next color pair in the palette.
void choosePalette(){
  if ( gLastPalette >= 15 ) {
    gLastPalette = 0;
  } else {
    gLastPalette+=2;
  }
}

// Fill a palette with some colors that my wife picked.
void setPalette(){
  /*
   * Jenn's colors RGB  0 0 81  BLUE
   * 0 100 100 Teal 006464
   * 60 100 100 Cool White 3C6464
   * 60 10 100 Violet 3C0A64
   * 60 0 50 Purple 3C0032
   * start white fades to Teal
   * violet to purple
   * teal to blue
   * red to blue
   */
  uint8_t r = random8(1, 255); // call it once first.
  fill_solid( gPalette, 16, CRGB::Red);
  gPalette[0] = CRGB( 60, 100, 100 ); // Jenn cool white
  gPalette[1] = CRGB( 0, 90, 90 );    // Jenn teal
  gPalette[2] = CRGB( 60, 10, 100 );  // Jenn violet
  gPalette[3] = CRGB( 60, 0, 50 );    // Jenn purple
  gPalette[4] = CRGB( 0, 0, 81);      // Jenn blue
  gPalette[5] = CRGB( 100, 0, 0);     // Red
  gPalette[6] = CRGB( 0, 0, 100);     // Blue
  gPalette[7] = CRGB( 120, 0, 120);
  // Random fill the rest.
  for (uint8_t i = 8; i<16; i++){
    gPalette[i] = CRGB(random8(3,100), random8(3,100), random8(3,100));
  }
}

// Walk the stairs adding random effects.
void walk() {
  static uint8_t valTop = 200;
  static int i = 0;
  // Pick two colors from the palette. 
  choosePalette();
  CRGB c1 = gPalette[gLastPalette];
  CRGB c2 = gPalette[gLastPalette+1];
  // chance of a random palette
  i = random8(1,255); i = random8(1,255); // bit of seeding
  if ( random8( 5 ) == 3 ){
    c1 = CRGB(random8(3,100),random8(3,100),random8(3,100));
    c2 = CRGB(random8(3,100),random8(3,100),random8(3,100));
  }
  // fix random Black palette.
  if ( (int(c1.r) + int(c1.g) + int(c1.b)) < 6 ){
    c1 = gPalette[2];
    c2 = gPalette[4];
  }
  CRGB trans = CRGB::Black;
  CRGB trans2 = CRGB::Black;
  //(r2-r1)/ticks * tick)
  
  for(gStair=0; gStair < NUM_LEDS; gStair+=2) {
    // Fade in step
    for (gBright=0; gBright<255; gBright = qadd8(gBright,4)) {
      // Fade in from off
      trans = blend(CRGB::Black,c1,gBright);
      leds[gUpDown[gStair]] = trans;
      leds[gUpDown[gStair + 1]] = trans;
      if (  gStair >= 2 ) { // shift last to stairs to the 2nd color.
        trans2 = blend(c1,c2,gBright);
        leds[gUpDown[gStair - 1]] = trans2;
        leds[gUpDown[gStair - 2]] = trans2;
      }
      FastLED.show();
      FastLED.delay(1); 
    }
  }
  
  // Hue shift the last two stairs
  for (gBright=0; gBright<255; gBright = qadd8(gBright,2)) {
    trans2 = blend(c1,c2,gBright);
    leds[gUpDown[NUM_LEDS - 1]] = trans2;
    leds[gUpDown[NUM_LEDS - 2]] = trans2;
    FastLED.show();
    FastLED.delay(1); 
  }

  // Delay + random effects
  
  randomEffect(c1, c2);
  
  // Turn off steps
  
  for(gStair=0; gStair < NUM_LEDS; gStair+=2) {
    for (gBright=0; gBright<=valTop; gBright++) { 
      leds[gUpDown[gStair]].fadeToBlackBy( valTop );
      leds[gUpDown[gStair + 1]].fadeToBlackBy( valTop );
      FastLED.show();
    }
    leds[gUpDown[gStair]] = CRGB( 0, 0, 0);
    leds[gUpDown[gStair + 1]] = CRGB( 0, 0, 0);
    FastLED.show();
  }
  gLightsOn = LOW;  
}

// Random effects for the walk() stair function.
void randomEffect(CRGB c1, CRGB c2){
  int i = 0, x = 0;
  i = random8(1,255); i = random8(1,255); // bit of seeding
  uint8_t var = random8( 1, 5 );
  CRGB trans2 = CRGB::Black;
  FastLED.delay(500); // Wait for it...
  switch (var) {
    case 1:
      //sparkle
      {
        for ( i=0; i<=500; i++){
          fill_solid(leds, NUM_LEDS, c2);
          addGlitter(80);
          FastLED.show();
          FastLED.delay(8.5);
        }        
        fill_solid(leds, NUM_LEDS, c2);
        FastLED.show();
      }
      break;
    case 2:
      //pulsate entire strip between colors
      {
        CRGBPalette16 z;
        z[0] = c2;
        z[1] = c1;
        z[2] = CRGB(random8(2,100),random8(2,100),random8(2,100));
        z[3] = c1;
        for (i=0; i<4; i++){
          for (x = 0; x<=255; x=qadd8(x,1)){
            trans2 = blend(z[i],z[i+1],x);
            fill_solid(leds, NUM_LEDS, trans2);
            FastLED.show();
            FastLED.delay(10);
          }
        }
      }
      break;
    case 3:
      //pulsate each stair strip between colors
      {
        CRGBPalette16 p;
        for (i=0; i<16; i+=2){
          p[i] = c1;
          p[i+1] = c2;
        }
        // Set final color
        p[8] = c1;
        p[9] = c1;
        for (i=0; i < 10; i++){
          CRGB cur1 = leds[0];
          CRGB cur2 = leds[2];
          for (uint8_t fade=0; fade<255; fade = qadd8(fade,2)) {
            for(gStair=0; gStair < NUM_LEDS; gStair+=4) {
              leds[gUpDown[gStair]] = blend(cur1,p[i],fade);
              if (gStair + 1 < NUM_LEDS){
                leds[gUpDown[gStair + 1]] = blend(cur1,p[i],fade);  
              }
              if (gStair + 2 < NUM_LEDS){
                leds[gUpDown[gStair + 2]] = blend(cur2,p[i+1],fade);  
              }
              if (gStair + 3 < NUM_LEDS){
                leds[gUpDown[gStair + 3]] = blend(cur2,p[i+1],fade);  
              } 
            }  
            FastLED.show();
            FastLED.delay(10);
          }
        }
      }
      break;
    default: 
      //Flash then solid 
      {
        for(gStair=0; gStair < NUM_LEDS; gStair+=2) {
          leds[gUpDown[gStair]] = CRGB( 100, 100, 100);
          leds[gUpDown[gStair + 1]] = CRGB( 100, 100, 100);
          FastLED.show();
        }
        for(gStair=0; gStair < NUM_LEDS; gStair+=2) {
          leds[gUpDown[gStair]] = c2;
          leds[gUpDown[gStair+1]] = c2;
          FastLED.show();
        }
        FastLED.show();
        FastLED.delay(7000);
      }
  } 
}

// Sparkle rainbow welcome give delay to calibrate pir sensors.  This also indicates if program crashed.
void welcomeRainbow(){
  for ( int i = 0; i < 500; i++ ){
    rainbowWithGlitter();
    FastLED.show();
    FastLED.delay(8.3);
    EVERY_N_MILLISECONDS( 20 ) { gHue++; }
  }
  for (int tick=0; tick < 64; tick++){ 
    for ( uint8_t i = 0; i < NUM_LEDS; i++ ){
      leds[i].fadeToBlackBy( 64 );
      FastLED.show();
      FastLED.delay(1);
    }
  }
}

// built-in FastLED rainbow, plus some random sparkly glitter
void rainbowWithGlitter() {
  rainbow();
  addGlitter(80);
}

// paint rainbow
void rainbow() {
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

// Add random glitter
void addGlitter( fract8 chanceOfGlitter) {
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB(100,100,100);
  }
}

// Candle flicker, blown out, + ember glow
void flicker(){
  static uint16_t i = 0;
  static uint8_t rnd = 0;
  uint8_t r = 0, g = 0, b = 0;
  int8_t stair = 0;
  gBright = 0;
  for(gStair=0; gStair < NUM_LEDS; gStair+=2) {
    for( i = 0; i <= 18; i++ ) {
      for (stair = 0; stair <= gStair; stair+=2) {
        rnd = random8(1, 4);
        if ( rnd == 2 ){
          gBright = random8(110,140);
          leds[gUpDown[stair]] = CHSV( 60, 200, gBright );
          leds[gUpDown[stair + 1]] = CHSV( 60, 200, gBright );
        }
        FastLED.show();
      }
    }
  }
  // Wait
  for ( i=0; i<=300; i++) {
    for( gStair = 0; gStair < NUM_LEDS; gStair+=2) {  
      rnd = random8(1, 4);
      if ( rnd == 2 ){
        gBright = random8(110,140);
        leds[gStair] = CHSV( 60, 200, gBright );
        leds[gStair+1] = CHSV( 60, 200, gBright );
      }
    }
    FastLED.show();
    FastLED.delay(25);
  }
  // Blow out candles and leave an ember.
  for(gStair=0; gStair < NUM_LEDS; gStair+=2) {
    rnd = random8(4, 6);
    r = rnd+1;
    g = rnd-2;
    leds[gUpDown[gStair]] = CRGB( r,g,0 );
    leds[gUpDown[gStair + 1]] = CRGB( r,g,0 );
    FastLED.show();
    FastLED.delay(50);
  }
  for ( i=0; i<=200; i++) {
    rnd = random8(0, NUM_LEDS-1);
    leds[gUpDown[rnd]].fadeToBlackBy( 5 );
    FastLED.show();
    FastLED.delay(50);
  }
  fill_solid(leds, NUM_LEDS, CRGB( 0, 0, 0 ));
  FastLED.show();
  gLightsOn = LOW;  
}

// Fade7 effect with each led: r,rb,b,bg,g,gr rgb(white)
void fade7(){
  const uint8_t mx = BRIGHTNESS;
  uint8_t r = mx;
  uint8_t g = 0;
  uint8_t b = 0;
  const uint8_t del = 25;
  boolean glitter = ( random8() < 120 );
  for (gStair=0; gStair < NUM_LEDS; gStair+=2){
    for (r=0; r<mx; r+=2) {
      leds[gUpDown[gStair]] = CRGB( r, g, b );
      leds[gUpDown[gStair + 1]] = CRGB( r, g, b );
      FastLED.show();
      FastLED.delay(1);
    }
  }
  for (b=0; b<mx; b++){
    fill_solid(leds, NUM_LEDS, CRGB( r, g, b ));
    if (glitter) addGlitter(80);
    FastLED.show();
    delay(del);
  }
  for (r=mx; r>0; r--){
    fill_solid(leds, NUM_LEDS, CRGB( r, g, b ));
    if (glitter) addGlitter(70);
    FastLED.show();
    delay(del);
  }
  for (g=0; g<mx; g++){
    fill_solid(leds, NUM_LEDS, CRGB( r, g, b ));
    if (glitter) addGlitter(60);
    FastLED.show();
    delay(del);
  }
  for (b=mx; b>0; b--){
    fill_solid(leds, NUM_LEDS, CRGB( r, g, b ));
    if (glitter) addGlitter(50);
    FastLED.show();
    delay(del);
  }
  for (r=0; r<mx; r++){
    fill_solid(leds, NUM_LEDS, CRGB( r, g, b ));
    if (glitter) addGlitter(40);
    FastLED.show();
    delay(del);
  }
  for (b=0; b<mx; b++){
    fill_solid(leds, NUM_LEDS, CRGB( r, g, b ));
    if (glitter) addGlitter(30);
    FastLED.show();
    delay(del);
  }
  for (b=mx; b>0; b--){
    g=b;
    fill_solid(leds, NUM_LEDS, CRGB( r, g, b ));
    if (glitter) addGlitter(10);
    FastLED.show();
    delay(del);
  }
  for (gStair=0; gStair < NUM_LEDS; gStair+=2){
    for (r=mx; r>1; r--) { 
      leds[gUpDown[gStair]] = CRGB( r, g, b );
      leds[gUpDown[gStair + 1]] = CRGB( r, g, b );
      FastLED.show();
      FastLED.delay(1); 
    }
    r = 0; g = 0; b = 0;
    leds[gUpDown[gStair]] = CRGB( r, g, b );
    leds[gUpDown[gStair + 1]] = CRGB( r, g, b );
    FastLED.show();
  }
  gLightsOn = LOW;  
}

