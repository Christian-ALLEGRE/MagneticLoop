# MagneticLoop
DIY Magnetic loop antenna for WARC bands (17m and 12m)<br/>
<br/>
This project show how to motorize the CV of a magnetic loop.<br/>
It contains :<br/>
  - The Fusion360 design of the step motor support as well as the STL file used to print it on my 3D printer.<br/>
  - The Arduino program used to drive the step motor<br/>
  - Some pictures of the final result.<br/>
<br/> 
The Arduino program drives a 28BYJ-48 step motor connected to the CV of my magnetic loop antenna.<br/>
<br/>
I setup this antenna on my balcony, and this program is used to tune the Antenna from my desktop.<br/>
<br/> 
It saves the current values in the Arduino EEPROM and restore them at boot time.<br/>
If nothing has moved while the Arduino was not connected, this allow to restart in the same state where is was when it shutdown.

Stored values: <br/>
  curPos: the current CV position<br/>
     dir: the last direction used (CW or CCW)<br/>
     jeu: the backlash value to add when changing direction (can be 0 to not manage backlash) <br/>
          this "jeu" value is not added to curPos.<br/>
<br/>
 This program uses input from the keyboard (in Arduino IDE), and understand the folowing commands:<br/>
   +: Move 5 steps CW (+++ moves 15 steps CW)<br/>
   -: Move 5 steps CCW<br/>
   6: Move 100 steps CW<br/>
   4: Move 100 steps CCW<br/>
   9: Move 200 steps CW<br/>
   7: Move 200 steps CCW<br/>
   .: Display current values<br/>
   0: Set current values as the ZERO values for 17m band (the antenna must be tuned on 17m)<br/>
   j  J : Activate or deactivate backlash management. backlash value is defined at compile time by define JEU<br/>
   1: jump to 12m band (value is defined at compile time by define B12M)<br/>
   3: jump to 17m band (value is defined at compile time by define B17M)<br/>

