
#include <Arduino.h>
#include <Encoder.h>
#include <Button.h>
#include <Potentiometer.h>
#include <SerialCommunication.h>
#include <ShiftRegister.h>


#define BROWSER_ENCODER_CLK 11
#define BROWSER_ENCODER_DT 12

#define DATA_PIN 11
#define CLK_PIN 12
#define LATCH_PIN 13

#define FOWARD 1
#define BACKWARD -1

int volumeBuffer = 0;

Encoder browserEncoder;
Button playPause;

Potentiometer volumeLeft;
Potentiometer volumeRight;
Potentiometer crossfader;

Potentiometer eqEffectLeft;
Potentiometer eqEffectRight;

ShiftRegister shiftRegister;

void setup() {
  Serial.begin(115200);
  Serial.println("starting...");

  browserEncoder.connect(BROWSER_ENCODER_CLK, BROWSER_ENCODER_DT);

  browserEncoder.setActionClockwise("jogwheel left", FOWARD);
  browserEncoder.setActionCounterClockwise("jogwheel left", BACKWARD);

  playPause.connect(9);
  playPause.setAction("play_pause left", 1);

  volumeLeft.connect(A0);
  volumeLeft.setSerialHeader("lvl_left");
  volumeLeft.isLogarithmic = true;
  
  volumeRight.connect(A1);
  volumeRight.setSerialHeader("lvl_right");
  volumeRight.isLogarithmic = true;

  crossfader.connect(A2);
  crossfader.setSerialHeader("crossfader");
  crossfader.isSwapped = true;
  crossfader.isLogarithmic = true;
  crossfader.reverseLogarithmic = true;


  eqEffectLeft.connect(A7);
  eqEffectLeft.isSwapped = true;
  eqEffectLeft.setSerialHeader("eq1_left");
  
  eqEffectRight.connect(A6);
  eqEffectRight.setSerialHeader("eq1_right");


  shiftRegister.connect(DATA_PIN, CLK_PIN, LATCH_PIN);

}

long binary_to_int(char *binary_string);

void loop() {

  volumeLeft.check();
  volumeRight.check();
  crossfader.check();

  eqEffectLeft.check();
  eqEffectRight.check();

  shiftRegister.write(1, HIGH);

/*
  char* array[] = {"10111111",  "11011111", "11101111",  "11110111"};
  //char* array[] = {"11111111", "00000000"};
  for (int i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
 
    // ST_CP LOW to keep LEDs from changing while reading serial data
    digitalWrite(latchPin, LOW);
 
    // Shift out the bits
    shiftOut(dataPin, clockPin, MSBFIRST, binary_to_int(array[i]));
 
    // ST_CP HIGH change LEDs
    digitalWrite(latchPin, HIGH);

    if(digitalRead(2) == LOW) {
      Serial.print("pressed - row: ");
      Serial.print(i + 1);
      Serial.println(", column: 1");
    }

    if(digitalRead(3) == LOW) {
      Serial.print("pressed - row: ");
      Serial.print(i + 1);
      Serial.println(", column: 2");
    }
 
    delay(10);
  }*/

  delay(1);
}



