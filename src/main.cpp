
#include <Arduino.h>
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


void setup() {

  Serial.begin(115200);

  
  shiftRegister.connect(DATA_PIN, CLK_PIN, LATCH_PIN);

  connectPlates();
  connectSliders();
  connectSinglePotentiometers();
  connectPotentiometerArray();
  initButtons();

  potentiometers.setIdleFunction(checkEncoders);
  buttonMatrix.setIdleFunction(checkEncoders);

}

void loop() {
  
  checkAll();
  checkEncoders();

  delay(1);
}

void connectPlates() {

  leftVinyl.connect(LEFT_VINYL_CLK, LEFT_VINYL_DT);

  leftVinyl.setActionClockwise("jogwheel left", FOWARD);
  leftVinyl.setActionCounterClockwise("jogwheel left", BACKWARD);

  rightVinyl.connect(RIGHT_VINYL_CLK, RIGHT_VINYL_DT);

  rightVinyl.setActionClockwise("jogwheel right", FOWARD);
  rightVinyl.setActionCounterClockwise("jogwheel right", BACKWARD);

}

void connectSliders() {

  volumeLeft.connect(A1);
  volumeLeft.setSerialHeader("lvl_L");
  volumeLeft.isLogarithmic = true;
  
  volumeRight.connect(A2);
  volumeRight.setSerialHeader("lvl_R");
  volumeRight.isLogarithmic = true;

  crossfader.connect(A0);
  crossfader.setSerialHeader("cfdr");
  crossfader.isSwapped = true;
  crossfader.isLogarithmic = true;
  crossfader.reverseLogarithmic = true;

}

void connectSinglePotentiometers() {

  eqEffectLeft.connect(A7);
  eqEffectLeft.isSwapped = true;
  eqEffectLeft.setSerialHeader("eq1_L");
  
  eqEffectRight.connect(A6);
  eqEffectRight.setSerialHeader("eq1_R");

  leftEffect.connect(A4);
  leftEffect.setSerialHeader("eff_slider_L");

  rightEffect.connect(A3);
  rightEffect.isSwapped = true;
  rightEffect.setSerialHeader("eff_slider_R");

}

void connectPotentiometerArray() {
  
  potentiometers.setMainConnection(A5, shiftRegister);

  potentiometers.connectPotenitometer(4, "eq2_L", true);
  potentiometers.connectPotenitometer(5, "eq2_R", false);
  potentiometers.connectPotenitometer(6, "eq3_L", true);
  potentiometers.connectPotenitometer(7, "eq3_R", false);
  
}

void initButtons() {
  int rowPins[] = {2, 3, 8, 9};
  int columnPins[] = {1, 3, 0, 2};

  buttonMatrix.connect(shiftRegister, columnPins, rowPins);
  buttonMatrix.setButtonHeader(0, 0, "L_PAD 1");
  buttonMatrix.setButtonHeader(1, 0, "L_PAD 2");
  buttonMatrix.setButtonHeader(2, 0, "L_PAD 3");
  buttonMatrix.setButtonHeader(3, 0, "L_PAD 4");

  buttonMatrix.setButtonHeader(0, 1, "L_SYNC");
  buttonMatrix.setButtonHeader(1, 1, "L_CUE");
  buttonMatrix.setButtonHeader(2, 1, "L_EFFECT");
  buttonMatrix.setButtonHeader(3, 1, "L_PLAY");

  buttonMatrix.setButtonHeader(0, 2, "R_PAD 1");
  buttonMatrix.setButtonHeader(1, 2, "R_PAD 2");
  buttonMatrix.setButtonHeader(2, 2, "R_PAD 3");
  buttonMatrix.setButtonHeader(3, 2, "R_PAD 4");

  buttonMatrix.setButtonHeader(0, 3, "R_EFFECT");
  buttonMatrix.setButtonHeader(1, 3, "R_SYNC");
  buttonMatrix.setButtonHeader(2, 3, "R_CUE");
  buttonMatrix.setButtonHeader(3, 3, "R_PLAY");
}

void checkAll() {

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

void checkEncoders() {

  leftVinyl.check();
  rightVinyl.check();

}



