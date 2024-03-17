
#include <Arduino.h>
#include "ardumidi/ardumidi.h"
#include <Encoder.h>
#include <Button.h>
#include <Potentiometer.h>
#include <PotentiometerArray.h>
#include <SerialCommunication.h>
#include <ShiftRegister.h>
#include <ButtonMatrix.h>

#define LEFT_VINYL_DT 6
#define LEFT_VINYL_CLK 7

#define RIGHT_VINYL_DT 4
#define RIGHT_VINYL_CLK 5

#define DATA_PIN 13
#define CLK_PIN 12
#define LATCH_PIN 11

#define FOWARD 1
#define BACKWARD -1

void checkEncoders();
void checkAll();
void connectPlates();
void connectSliders();
void connectSinglePotentiometers();
void connectPotentiometerArray();
void initButtons();

Encoder leftVinyl;
Encoder rightVinyl;

Potentiometer volumeLeft;
Potentiometer volumeRight;
Potentiometer crossfader;

Potentiometer eqEffectLeft;
Potentiometer eqEffectRight;

Potentiometer leftEffect;
Potentiometer rightEffect;

PotentiometerArray potentiometers;

ShiftRegister shiftRegister;

ButtonMatrix buttonMatrix;

bool *add;
void setup()
{

  Serial.begin(115200);

  shiftRegister.connect(DATA_PIN, CLK_PIN, LATCH_PIN);

  connectPlates();
  connectSliders();
  initButtons();
  connectSinglePotentiometers();
  connectPotentiometerArray();


  potentiometers.setIdleFunction(checkEncoders);
  buttonMatrix.setIdleFunction(checkEncoders);
}

void loop()
{
  checkAll();
  checkEncoders();

  delay(1);
}

void connectPlates()
{

  leftVinyl.connect(LEFT_VINYL_CLK, LEFT_VINYL_DT);
  leftVinyl.reverseDir();
  leftVinyl.setControl(0x25);

  rightVinyl.connect(RIGHT_VINYL_CLK, RIGHT_VINYL_DT);
  rightVinyl.setControl(0x26);
}

void connectSliders()
{

  volumeLeft.connect(A1);
  volumeLeft.isLogarithmic = true;
  volumeLeft.setControllerChange(0xE);

  volumeRight.connect(A2);
  volumeRight.isLogarithmic = true;
  volumeRight.setControllerChange(0xF);

  crossfader.connect(A0);
  crossfader.isSwapped = true;
  crossfader.isLogarithmic = true;
  crossfader.reverseLogarithmic = true;
  crossfader.setControllerChange(0x10);
}

void connectSinglePotentiometers()
{

  eqEffectLeft.connect(A7);
  eqEffectLeft.isSwapped = true;
  eqEffectLeft.setControllerChange(0x11);

  eqEffectRight.connect(A6);
  eqEffectRight.setControllerChange(0x14);

  leftEffect.connect(A4);
  leftEffect.setControllerChange(0x17);
  rightEffect.connect(A3);
  rightEffect.isSwapped = true;
  rightEffect.setControllerChange(0x18);
}

void connectPotentiometerArray()
{

  potentiometers.setMainConnection(A5, shiftRegister);

  potentiometers.connectPotenitometer(4, 0x12, true);
  potentiometers.connectPotenitometer(5, 0x15, false);
  potentiometers.connectPotenitometer(6, 0x13, true);
  potentiometers.connectPotenitometer(7, 0x16, false);
}

void initButtons()
{
  int rowPins[] = {2, 3, 9, 8};
  int columnPins[] = {1, 3, 0, 2};

  buttonMatrix.init(shiftRegister, columnPins, rowPins);
}

void checkAll()
{

  volumeLeft.check();
  volumeRight.check();
  crossfader.check();

  eqEffectLeft.check();
  eqEffectRight.check();

  leftEffect.check();
  rightEffect.check();
  potentiometers.check();
  buttonMatrix.check();
}

void checkEncoders()
{

  leftVinyl.check();
  rightVinyl.check();
}
