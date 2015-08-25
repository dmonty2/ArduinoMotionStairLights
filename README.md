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

Mounting:
* PIR sensor glued into a blank wall face-plate with round hole drilled into it.
* LED tube is:
  * a pipe cut in 1/2 and screwed to the wall. 
  * led and wiring inside the 1/2 pipe.
  * Clear tubing is sanded to make it frosty & a slit is cut in it.
  * The clear-frosted tube is then sliped over top of the 1/2 pipe.

You can find examples of wiring online:
* https://www.google.ca/search?q=arduino+pir&tbm=isch
* https://learn.adafruit.com/adafruit-neopixel-uberguide/power
* https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library

Video:
<a href="https://vimeo.com/135328344" target="_blank" ><img src="https://i.vimeocdn.com/video/529290528.webp?mw=500&mh=281" width="500" height="281" /></a>

Mounting Conduit:
![led_conduit](https://cloud.githubusercontent.com/assets/1381071/9459073/b65b1010-4aab-11e5-9349-805730a26c46.jpg)

* Cost for controller and lighting:
  * $10 - 1 meter LED lights ws2812b 30 leds/m.
  * $7 - microcontroller - arduino uno.
  * $3 - 2x Passive infrared motion sensors.
  * $5.50 - 5v 3A switching power supply.
  * Free - repuposed 1000uf capacitor

Subtotal lighting: $25

* Cost for mounting hardware:
  * $3 - metal electrical conduit tube - we cut it in half and screwed it to the wall.
  * $15 - Vinal Tubing - cut a slit in it and sanded it down & slid it over top of the half-conduit.
  * $3 - 2x blank light switch cover - drilled a hole in it to mount the PIR sensors.
  * $7 - Access panel - so I could hide the microcontroller inside the wall.

Subtotal mounting: $28

TOTAL $53

TODO/Whishlist:
* Make it so you can change the # of LEDs per stair.

