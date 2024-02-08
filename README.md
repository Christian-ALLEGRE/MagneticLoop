# MagneticLoop
DIY Magnetic loop antenna for WARC bands (17m and 12m)

This project show how to motorize the CV of my magnetic loop.
It contains :
  - The Fusion360 design of the step motor support as well as the STL file used to print it on my 3D printer
  - The Arduino program used to drive the step motor
  - Some pictures of the final result.
 
The Arduino program drives a 28BYJ-48 step motor connected to the CV of my magnetic loop antenna.

I setup this antenna on my balcony, and this program is used to tune the Antenna from my desktop.
 
It saves the current values in the Arduino EEPROM and restore them at boot time.
If nothing has moved while the Arduino was not connected, this allow to restart in the same state where is was when it shutdown.

Stored values: 
  curPos: the current CV position
     dir: the last direction used (CW or CCW)
     jeu: the backlash value to add when changing direction (can be 0 to not manage backlash) 
          this "jeu" value is not added to curPos.

 This program uses input from the keyboard (in Arduino IDE), and understand the folowing commands:
   +: Move 5 steps CW (+++ moves 15 steps CW)
   -: Move 5 steps CCW
   6: Move 100 steps CW
   4: Move 100 steps CCW
   9: Move 200 steps CW
   7: Move 200 steps CCW
   .: Display current values
   0: Set current values as the ZERO values for 17m band (the antenna must be tuned on 17m)
   j  J : Activate or deactivate backlash management. backlash value is defined at compile time by define JEU
   1: jump to 12m band (value is defined at compile time by define B12M)
   3: jump to 17m band (value is defined at compile time by define B17M)

