* SkullDougery - 2024
* Buld with platformio.
* * Build using Arduino environment. esp32doit platform.
* Libraries added:
    ESP32Servo  (appears in platformio as  madhephaestus/ESP32Servo@^1.1.2)

JAW MOTION NOTES:
(1) Uses servo for jaw motion:
      Use FFT on source to only look at intensity of limits ranges:
           above 2.5k - 'ssss'...
	   below 400  - Exposives
	   in between - use average sound level?
(2a) Decode MP3 from source (8khz mp3 on spiff or maybe external SD card)
(2b) Use raw input from a microphone
(3) Output via I2s to speaker driver.
NOTE: Jaw motion - SLOW  compared to bit rate - each phoneme is <.5 milliseconds
(4) Joystick for tilt/nod/rotate?

ADDITIONAL:
Rotate (servo)   (possibly follow 'victum')
Eyes (Left and Right) (possibly brightness follows voice?)

NOD  (nod and tilt a combination of two servos) (random?)
TILT (nod and tilt a combination of two servos) ( overall tempo or intensity?)
