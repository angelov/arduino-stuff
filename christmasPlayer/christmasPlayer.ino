/**
 * Info:
 *  - The info about the songs (melodies, number of notes and notes durations) and some part of the
 *    code for playing them were taken from http://arduinomelodies.blogspot.mk/.
 *  - Using the modified LiquidCrystal Library from https://bitbucket.org/fmalpartida/new-liquidcrystal/
 */

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include "pitches.h"

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int melodies[][100] = {

  { NOTE_G4, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4,
    NOTE_A4, NOTE_D5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_G4,
    NOTE_G4, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A5, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_D5, NOTE_B4, NOTE_C5,
    NOTE_G4, NOTE_C5, NOTE_C5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_A4,
    NOTE_A4, NOTE_D5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_B4, NOTE_G4,
    NOTE_G4, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A5, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_D5, NOTE_B4, NOTE_C5,
    NOTE_G4, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_B4, NOTE_B4, NOTE_C5, NOTE_B4, NOTE_A4, NOTE_G4,
    NOTE_D5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_C5, NOTE_C5, NOTE_G5, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_D5, NOTE_B4, NOTE_C5
  },

  { NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_G4, NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4,
    NOTE_A4, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_B4, NOTE_G5, NOTE_G5, NOTE_F5, NOTE_D5, NOTE_E5,
    NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_G4, NOTE_G4, NOTE_E5, NOTE_D5, NOTE_C5, NOTE_A4,
    NOTE_A4, NOTE_F5, NOTE_E5, NOTE_D5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_D5, NOTE_C5, NOTE_G5,
    NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5, NOTE_E5,
    NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_D5, NOTE_D5, NOTE_E5, NOTE_D5, NOTE_G5,
    NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_G5, NOTE_C5, NOTE_D5, NOTE_E5,
    NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_F5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_G5, NOTE_G5, NOTE_F5, NOTE_D5, NOTE_C5
  }

};

int numberOfNotes[] = {84, 98};

int noteDurations[][100] = {

  {
    4, 4, 8, 8, 8, 8, 4, 4,
    4, 4, 8, 8, 8, 8, 4, 4,
    4, 4, 8, 8, 8, 8, 4, 4, 8, 8, 4, 4, 4, 2,
    4, 4, 8, 8, 8, 8, 4, 4,
    4, 4, 8, 8, 8, 8, 4, 4,
    4, 4, 8, 8, 8, 8, 4, 4, 8, 8, 4, 4, 4, 2,
    4, 4, 4, 4, 2, 4, 4, 4, 4, 2,
    4, 4, 8, 8, 8, 8, 4, 4, 8, 8, 4, 4, 4, 2,
  },

  { 8, 8, 8, 8, 2, 8, 8, 8, 8, 2,
    8, 8, 8, 8, 2, 8, 8, 8, 8, 2,
    8, 8, 8, 8, 2, 8, 8, 8, 8, 2,
    8, 8, 8, 8, 8, 8, 8, 16, 16, 8, 8, 8, 8, 4, 4,
    8, 8, 4, 8, 8, 4, 8, 8, 8, 8, 2,
    8, 8, 8, 16, 16, 8, 8, 8, 16, 16, 8, 8, 8, 8, 4, 4,
    8, 8, 4, 8, 8, 4, 8, 8, 8, 8, 2,
    8, 8, 8, 16, 16, 8, 8, 8, 16, 16, 8, 8, 8, 8, 2,
  }

};

String titles[] = {"Merry Christmas", "Jingle Bells"};

int nextSongButton = 12;
int playStopButton = 13;

int latchPin = 11;
int dataPin = 10;
int clkPin = 9;

int currentSong = 0;
int playing = 0;

int numberOfSongs = 0;

void setup() {

  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clkPin, OUTPUT);

  pinMode(nextSongButton, INPUT);
  pinMode(playStopButton, INPUT);

  lcd.begin(16, 2);
  lcd.backlight();

  displaySong(0);
  turnOffLeds();
  numberOfSongs = (int)(sizeof(melodies) / sizeof(melodies[0]));
}

void loop() {

  int displayNextSong = digitalRead(nextSongButton);

  if (displayNextSong == HIGH) {
    currentSong++;

    if (currentSong == numberOfSongs) {
      currentSong = 0;
    }

    displaySong(currentSong);
  }

  int changePlayingState = digitalRead(playStopButton);

  if (changePlayingState == HIGH) {
    if (playing == 1) {
      playing = 0;
    } else {
      playing = 1;
    }
  }

  if (playing == 1) {
    for (int thisNote = 0; thisNote < numberOfNotes[currentSong];  thisNote++) {
      int noteDuration = 1000 / noteDurations[currentSong][thisNote];
      tone(3, melodies[currentSong][thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      noTone(3);

      shuffleLeds();

      changePlayingState = digitalRead(playStopButton);

      if (changePlayingState == HIGH) {
        playing = 0;
        turnOffLeds();
        break;
      }

    }

  }

  delay(200);
}

void displaySong(int id) {
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print(id + 1);
  lcd.print(". ");
  lcd.print(titles[id]);
}

void shuffleLeds() {
  shiftNumber(random(0, 257));
}

void turnOffLeds() {
  shiftNumber(0);
}

void shiftNumber(int num) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clkPin, MSBFIRST, num);
  digitalWrite(latchPin, HIGH);
}

