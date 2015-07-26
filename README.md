# ArduinoMotionStairLights
PIR Motion activated RGB led stair lights for Arduino.

Goal of this project is to have animated RGB WS2812B stair lights.  The lights are activated by 2 PIR Motion sensors located at the top and bottom of the stairs.

WS2812B addressable led string usually comes in a strip of 30 LEDs.  I put 2 LEDs per stair which gave me 2-spare pairs in case an led pair needs to be replaced.

It currently requires FastLED 3.1 Arduino Library
https://github.com/FastLED/FastLED/tree/FastLED3.1

This is now ready for Beta testing.

TODO/Whishlist:
* Make it so you can change the # of LEDs per stair.

