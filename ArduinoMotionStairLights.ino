/*
 * Description: Motion activated stair lights.
 * Author: Dean Montgomery
 * Version: 0.1 (Alpha)
*/

#include "FastLED.h"
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
#define NUM_LEDS 26
#define LEDS_PER_STAIR 2        // Number of Leds per stair.
#define BRIGHTNESS  120         // 0...255  ( needed ?)
#define UPDATES_PER_SECOND 100
#define PIN_LED 3               // LED Data pin
#define PIN_PIR_DOWN 5          // PIR Downstairs Pin
#define PIN_PIR_UP 7            // PIR Upstairs Pin
#define STEP_DELAY 200          //ms = 1/5th second. ( needed ?)
#define GO_UP -1                // Direction control - Arduino at top of stairs
#define GO_DOWN 1               // Direction control - Arduino at top of stairs
int8_t lightsOn = LOW;          // track lights on/off
uint8_t gHue = 0;
int8_t gStair = 0;
uint8_t gBright = 0;
int8_t gStairStart = 0;
int8_t gStairEnd = 0;
uint8_t gUpDown[NUM_LEDS];      // directional array to walk/loop up or down stairs.
  
CRGB leds[NUM_LEDS];




void setup() {
  delay (3000); // Power Up 3 second safety delay.
  Serial.begin(9600);
  FastLED.addLeds<WS2812B, PIN_LED, GRB>(leds, NUM_LEDS);
  //FastLED.setBrightness(BRIGHTNESS);
  //Serial.print("calibrating sensor ");
  pinMode(PIN_PIR_DOWN, INPUT);
  pinMode(PIN_PIR_UP, INPUT);
  digitalWrite(PIN_PIR_DOWN, LOW);
  digitalWrite(PIN_PIR_UP, LOW);
  Serial.println(" done");
  welcomeRainbow();  // rainbow - give time for PIR sensors.
  setUpDown(GO_DOWN);
  Serial.println("SENSOR ACTIVE");
}

// Main Loop track PIR sensors.
void loop() {
  // Walk Down.
  if ( digitalRead(PIN_PIR_UP) == HIGH ){
    if ( lightsOn == LOW ) {
      lightsOn = HIGH;
      setUpDown(GO_DOWN);
      // TODO: Add random pattern picker.
      fade7();
      //walk(1);
      //flicker(1);
      delay(50);
    }
  }
  // Walk Up.
  if ( digitalRead(PIN_PIR_DOWN) == HIGH  ){
    if ( lightsOn == LOW ) {
      lightsOn = HIGH;
      setUpDown(GO_UP);
      fade7(); //Up
      //walk(-1); // Up
      //flicker(-1);
      delay(50);
    }
  }
}

void setUpDown(int8_t upDownDir){
  // setup walking gUpDown array in forward: 0,1,2,3... or reverse:  ...3,2,1,0
  gStairStart = 0;
  if (upDownDir == GO_UP){
    for ( gStair = NUM_LEDS -1; gStair >= 0; gStair-- ){
      gUpDown[gStair] = gStairStart++;
    }
  } else {
    for ( gStair = 0; gStair <= NUM_LEDS; gStair++ ){
      gUpDown[gStair] = gStairStart++;
    }  
  }
  //printUpDown();
}

void printUpDown(){
  for (gStair = 0; gStair < NUM_LEDS; gStair++){
    Serial.print (gStair);
    Serial.print (" " );
    Serial.println (gUpDown[gStair]);
  }
}

void welcomeRainbow(){
  // Sparkle rainbow welcome give delay to calibrate pir sensors.  This also indicates if program crashed.
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



void walk(int8_t dir) {
  Serial.print("Walk dir: ");
  Serial.println(dir);
  static uint8_t hueStart = 208; //purple;
  static uint8_t hueEnd = 160;
  static uint8_t hueShift = 0;
  static uint8_t brightTop = 200;
  static int i = 0;
  setStartEnd(dir);
  
  for(gStair=gStairStart; (dir == 1 && gStair < gStairEnd ) || (dir == -1 && gStair >= gStairEnd && gStair <= gStairStart+1 ); gStair+=(2*dir)) {
    // Fade in step
    hueShift = hueStart;
    for (gBright=0; gBright<=brightTop; gBright+=2) {
      leds[gStair] = CHSV( hueStart, 204, gBright );  // purple
      leds[gStair + 1*dir] = CHSV( hueStart, 204, gBright );  // purple
      if (  (dir == 1 && gStair >= 2) || (dir == -1 && gStair <= (gStairStart-2)) ) { // slide hue from pruple to blue on previous stair
        //hueShift = hueShift + ((hueEnd-hueStart)/brightTop*gBright); //=D1+((160-240)/200*10)
        if ( hueShift >= hueEnd ) {
          hueShift--;
          leds[gStair - 1*dir] = CHSV( hueShift, 204, 200 );
          leds[gStair - 2*dir] = CHSV( hueShift, 204, 200 );
        }
      }
      FastLED.show();
      //FastLED.delay(1); 
    }
    Serial.print(gStair);
    Serial.println(" step");
  }
  
  Serial.println("Last Stairs");
  hueShift = hueStart;
  for (gBright=0; gBright<=brightTop; gBright+=2) {
    if ( hueShift >= hueEnd )
    //hueShift = 1/(hueShift + ((hueEnd-hueStart)/brightTop*bright));
    hueShift--;
    leds[gStairEnd] = CHSV( hueShift, 204, 200 );
    leds[gStairEnd - 1*dir] = CHSV( hueShift, 204, 200 );
    FastLED.show();
    //FastLED.delay(1); 
  }
  //Flash
  FastLED.delay(500); 
  for(gStair=gStairStart; (dir == 1 && gStair <= gStairEnd ) || (dir == -1 && gStair >= gStairEnd && gStair <= gStairStart+1 ); gStair+=(2*dir)) {
    leds[gStair] = CRGB( 100, 100, 100);
    leds[gStair + 1*dir] = CRGB( 100, 100, 100);
    FastLED.show();
  }
  for(gStair=gStairStart; (dir == 1 && gStair <= gStairEnd ) || (dir == -1 && gStair >= gStairEnd && gStair <= gStairStart+1 ); gStair+=(2*dir)) {
    leds[gStair] = CHSV( hueEnd, 204, 200 );
    leds[gStair+1*dir] = CHSV( hueEnd, 204, 200 );
    FastLED.show();
  }
  
  
  Serial.println("Delay");
  // Stay on for a bit
  fill_solid(leds, NUM_LEDS, CHSV( hueEnd, 204, 200 ));
  //addGlitter(70);
  FastLED.show();
  FastLED.delay(5000);
  
  // Turn off steps
  Serial.println("Steps Off");
  for(gStair=gStairStart; (dir == 1 && gStair <= gStairEnd ) || (dir == -1 && gStair >= gStairEnd && gStair <= gStairStart+1 ); gStair+=(2*dir)) {
    // Fade in step
    //hueShift = hueStart;
    for (gBright=200; gBright>=1; gBright--) { 
      leds[gStair] = CHSV(hueEnd, 204, gBright);
      leds[gStair + 1*dir] = CHSV(hueEnd, 204, gBright);
      FastLED.show();
      FastLED.delay(1); 
    }
    leds[gStair].setHSV( 0, 0, 0);
    leds[gStair + 1*dir].setHSV( 0, 0, 0);
    FastLED.show();
  }
  lightsOn = LOW;  
  
  Serial.println("Done");
}

// startup rainbow
void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}
void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::Grey;
  }
}

// Candle flicker.
void flicker(int8_t dir){
  setStartEnd(dir);
  static uint16_t i = 0;
  static uint8_t rnd = 0;
  uint8_t stair = 0;
  gBright = 0;
  for(gStair=gStairStart; (dir == 1 && gStair < gStairEnd ) || (dir == -1 && gStair >= gStairEnd && gStair <= gStairStart+1 ); gStair+=(2*dir)) {
    for( i = 0; i <= 18; i++ ) {
      for (stair = gStairStart; (dir == 1 && stair <= gStair ) || ( dir == -1 && stair >= gStair && stair <= gStairStart+1); stair+=(2*dir)) {
        rnd = random8(1, 4);
        if ( rnd == 2 ){
          gBright = random8(90,140);
          leds[stair] = CHSV( 60, 200, gBright );
          leds[stair + 1*dir] = CHSV( 60, 200, gBright );
        }
        FastLED.show();
      }
    }
  }
  // Wait
  Serial.println("Wait");
  for ( i=0; i<=300; i++) {
    for( gStair = 0; gStair < NUM_LEDS; gStair+=2) {  
      rnd = random8(1, 4);
      if ( rnd == 2 ){
        gBright = random8(90,140);
        leds[gStair] = CHSV( 60, 200, gBright );
        leds[gStair+1] = CHSV( 60, 200, gBright );
      }
    }
    FastLED.show();
    FastLED.delay(25);
  }
  Serial.println("Shutdown");
  // Shutdown
  for(gStair=gStairStart; (dir == 1 && gStair <= gStairEnd ) || (dir == -1 && gStair >= gStairEnd && gStair <= gStairStart+1 ); gStair+=(2*dir)) {
    for( i = 0; i <= 18; i++) {
      for (stair = gStair; (dir == 1 && stair <= gStairEnd ) || ( dir == -1 && stair >= gStair && stair <= gStairStart+1); stair+=(2*dir)) {
        rnd = random8(1, 4);
        if ( rnd == 2 ){
          gBright = random8(90,140);
          leds[stair] = CHSV( 60, 200, gBright );
          leds[stair + 1*dir] = CHSV( 60, 200, gBright ); 
          leds[gStair] = CRGB( 0,0,0 );
          leds[gStair + 1*dir] = CRGB( 0,0,0 );
        }
        FastLED.show();
      }
    }
    leds[gStair] = CRGB( 0,0,0 );
    leds[gStair + 1*dir] = CRGB( 0,0,0 );
    FastLED.show();
    FastLED.delay(25);
  }
  lightsOn = LOW;  
}


// Fade7 effect.
void fade7(){
  //setStartEnd(dir);
  Serial.println("Fade7");
  //Serial.println(dir);
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
  lightsOn = LOW;  
}

void setStartEnd(int8_t dir){
  if (dir == 1){ //Down
    gStairStart = 0;
    gStairEnd = NUM_LEDS-1;
  } else {
    gStairStart = NUM_LEDS-1;
    gStairEnd = 0;
  }
}
