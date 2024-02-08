// MoteurCV.ino
//
// Auteur: F4LAA 
// Date : 2024/02/07 
//
// This Arduino program drives a 28BYJ-48 step motor connected to the CV of my magnetic loop antenna.
//
// I setup this antenna on my balcony, and this program is used to tune the Antenna from my desktop.
// 
// It saves the current values in the Arduino EEPROM and restore them at boot time.
// If nothing has moved while the Arduino was not connected, 
// this allow to restart in the same state where is was when it shutdown.
//
// Stored values: 
//   curPos: the current CV position
//      dir: the last direction used (CW or CCW)
//      jeu: the backlash value to add when changing direction (can be 0 to not manage backlash) 
//           this value is not added to curPos.
//
// This program uses input from the keyboard (in Arduino IDE), and understand the folowing commands:
//   +: Move 5 steps CW (+++ moves 15 steps CW)
//   -: Move 5 steps CCW
//   6: Move 100 steps CW
//   4: Move 100 steps CCW
//   9: Move 200 steps CW
//   7: Move 200 steps CCW
//   .: Display current values
//   0: Set current values as the ZERO values for 17m band (the antenna must be tuned on 17m)
//   j / J : Activate or deactivate backlash management. backlash value is defined at compile time by define JEU
//   1: jump to 12m band (value is defined at compile time by define B12M)
//   3: jump to 17m band (value is defined at compile time by define B17M)
//
// Based on :
//   Bounce.pde
//   -*- mode: C++ -*-
//
//   Make a single stepper bounce from one limit to another
//
//   Copyright (C) 2012 Mike McCauley
//   $Id: Random.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $
#include <AccelStepper.h>
#include <EEPROM.h>

// Définitions des broches du moteur:
#define motorPin1  8      // IN1 on the ULN2003 driver
#define motorPin2  9      // IN2 on the ULN2003 driver
#define motorPin3  10     // IN3 on the ULN2003 driver
#define motorPin4  11     // IN4 on the ULN2003 driver

// Définit le type d'interface AccelStepper; Moteur 4 fils en mode demi-pas:
#define MotorInterfaceType 8

// Initialisation avec la séquence de broches IN1-IN3-IN2-IN4 pour utiliser la bibliothèque AccelStepper avec le moteur pas à pas 28BYJ-48:
AccelStepper stepper = AccelStepper(MotorInterfaceType, motorPin1, motorPin3, motorPin2, motorPin4);

#define LED 13
// Measures CV position for 17m band
#define B17M -240
// Measures CV position for 12m band
#define B12M -40

// Stored in EEprom
int curPos;  // Current position
int dir;     // last direction used
#define JEU 90
int jeu;     // backlash
int nDir;    // not saved in EEProm

void eepromSave()
{
  EEPROM.put(0, curPos);
  EEPROM.put(2, dir);
  EEPROM.put(4, jeu);
  Serial.print("Saved in EEProm: dir=");
  Serial.print(dir);
  Serial.print(" curPos=");
  Serial.print(curPos);
  Serial.print(" jeu=");
  Serial.println(jeu);
}

void eepromRestore()
{
  EEPROM.get(0, curPos);
  EEPROM.get(2, dir);
  EEPROM.get(4, jeu);
  nDir = dir;
  Serial.print("EEProm::dir=");
  Serial.print(dir);
  Serial.print(" curPos=");
  Serial.print(curPos);
  Serial.print(" jeu=");
  Serial.println(jeu);
}

void setup()
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT); // Used to see if Arduino was not booted by HF

  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(100);
  stepper.setAcceleration(100);

  // Valeur a prendre dans l'EEPROM
  eepromRestore(); // curPos, dir et jeu
  stepper.setCurrentPosition(curPos);
}

// // If at the end of travel go to the other end
// if (stepper.distanceToGo() == 0)
//   stepper.moveTo(-stepper.currentPosition());

void CVMove(int n)
{
  // Gestion du jeu
  if (dir == 0)
  {
    if (n > 0)
      dir = 1;
    else 
      dir = -1;
    nDir = dir;
  }
  else
  {
    // dir was defined
    if (n > 0)
      nDir = 1;
    else 
      nDir = -1;
    if (nDir != dir)  
    {
      // Rattrapage du jeu
      if (nDir == 1)
        n += jeu;
      else 
        n -= jeu;
    }
  }

  // Déplacement du CV
  curPos += n; 
  stepper.runToNewPosition(curPos);

  // On ne compte pas le jeu dans curPos
  if (nDir != dir)  
  {
    // Rattrapage du jeu
    if (nDir == 1)
      curPos -= jeu;
    else 
      curPos += jeu;
  }
  dir = nDir;
  eepromSave();
}

char cmd;
int cpt=0;
bool state = false;
void loop()
{
  // LED management (show that Arduino is still alive)
  state = !state;
  digitalWrite(LED, state);
  delay(250);
    
  // Commands management  
  cmd = ' ';
  if (Serial.available() > 0) {
    // read incoming serial data:
    cmd = Serial.read();
    switch(cmd)
    {
      case '.': // Show curPos, jeu and dir
        Serial.print("curPos=");
        Serial.print(curPos);
        Serial.print(" Dir=");
        Serial.print(dir);
        Serial.print(" Jeu=");
        Serial.println(jeu);
        break;

      case '0': // ReInit eeprom (Bande 17M avec ROS = 1:1)
        curPos = B17M;
        stepper.setCurrentPosition(curPos);
        eepromSave();
        break;

      // CV jump  
      case '1':
        // Bande 17m
        CVMove(B17M - curPos);
        break;

      case '3':
        // Bande 12m
        CVMove(B12M - curPos);
        break;

      // backlash management        
      case 'j':
      case 'J':
        if (jeu == JEU)
          jeu = 0;
        else
          jeu = JEU;
        Serial.print(" Jeu set to: ");
        Serial.println(jeu);
        break;

      // CV movements  
      case '+':
        CVMove(5);
        break;
      case '-':
        CVMove(-5);
        break;

      case '6':
        CVMove(100);
        break;
      case '4':
        CVMove(-100);
        break;

      case '9':
        CVMove(200);
        break;
      case '7':
        CVMove(-200);
        break;
    }
  }
}
