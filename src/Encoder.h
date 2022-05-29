#include <Arduino.h>
#include <SerialCommunication.h>

class Encoder {

    public:

        void connect(int clkPin, int dtPin) {
            clockPin = clkPin;
            dataPin = dtPin;
            pinMode(clockPin, INPUT);
            pinMode(dataPin, INPUT);
            clockLastState = digitalRead(clkPin);
        }

        void setActionClockwise(String header, int value) {
            clockwiseValue = value;
            clockwiseHeader = header;
        }

        void setActionCounterClockwise(String header, int value) {
            counterClockwiseValue = value;
            counterClockwiseHeader = header;
        }

        void check() {
            int clockValue = digitalRead(clockPin);
            if (clockValue != clockLastState) {
                if (digitalRead(dataPin) != clockValue) {
                    counter++;
                    if(counter % 2 == 0) {
                        SerialCommunication::sendCommand(counterClockwiseHeader, counterClockwiseValue);
                    }
                } else {
                    counter--;
                    if(counter % 2 == 0) {
                        SerialCommunication::sendCommand(clockwiseHeader, clockwiseValue);
                    }
                }
            }
              clockLastState = clockValue;
        }
        
        
    private:
        int clockPin;
        int dataPin;
        int dataLastState;
        int clockLastState;
        int counter = 0;

        int clockwiseValue;
        int counterClockwiseValue;
        String clockwiseHeader;
        String counterClockwiseHeader;
};