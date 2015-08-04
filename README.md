# ArduinoMotionStairLights
PIR Motion activated RGB led stair lights for Arduino.

Goal of this project is to have animated RGB WS2812B stair lights.  The lights are activated by 2 PIR Motion sensors located at the top and bottom of the stairs.

WS2812B addressable led string usually comes in a strip of 30 LEDs.  I put 2 LEDs per stair which gave me 2-spare pairs in case an led pair needs to be replaced.

It currently requires FastLED 3.1 Arduino Library
https://github.com/FastLED/FastLED/tree/FastLED3.1

This is now ready for Beta testing.


Components:
* 5v 3A switching power supply.
* ws2812b led strip
* 2x pir sensors
* 1000uF capacitor.
Wireing is pretty simple:
* All devices share the same 5V power.
* 3 data pins are used: for the two pir sensors and the ws2812b strip. You may want to choose different data pins - the defaults are marked at the top of the file.

You can find examples of wiring online:
https://www.google.ca/search?q=arduino+pir&tbm=isch
https://learn.adafruit.com/adafruit-neopixel-uberguide/power
https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library


<a href="https://vimeo.com/135328344" target="_blank" ><img src="https://i.vimeocdn.com/video/529290528.webp?mw=500&mh=281" width="500" height="281" /></a>

Known Bug:
* Random color pickers sometimes choose RGB=000 (black/off).  I need to add some checks to the code to ensure a minimal color level.


TODO/Whishlist:
* Make it so you can change the # of LEDs per stair.

