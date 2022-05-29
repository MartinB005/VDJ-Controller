
#include <Arduino.h>
#include <Encoder.h>
#include <Button.h>
#include <SerialCommunication.h>


#define BROWSER_ENCODER_CLK 11
#define BROWSER_ENCODER_DT 12

#define FOWARD 1
#define BACKWARD -1

int volumeBuffer = 0;

Encoder browserEncoder;
Button playPause;

void setup() {
  Serial.begin(115200);
  Serial.println("starting...");

  browserEncoder.connect(BROWSER_ENCODER_CLK, BROWSER_ENCODER_DT);

  browserEncoder.setActionClockwise("jogwheel left", FOWARD);
  browserEncoder.setActionCounterClockwise("jogwheel left", BACKWARD);

  playPause.connect(9);
  playPause.setAction("play_pause left", 1);
}

void loop() {
  browserEncoder.check();
  playPause.check();
  delay(1);
}

