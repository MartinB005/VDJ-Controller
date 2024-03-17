#include <Arduino.h>
#include <SerialCommunication.h>

#define VELOCITY_FORWARD 0x72
#define VELOCITY_BACKWARD 0x12

class Encoder {

    public:

        void connect(int clkPin, int dtPin) {
            clockPin = clkPin;
            dataPin = dtPin;
            pinMode(clockPin, INPUT);
            pinMode(dataPin, INPUT);
            clockLastState = digitalRead(clkPin);
        }

        void setControl(int cc) {
            this->control = cc;
        }

        void reverseDir() {
            reversed = true;
        }

        void check() {
            int clockValue = digitalRead(clockPin);
            if (clockValue != clockLastState) {
                if (digitalRead(dataPin) == 1) {
                    counter++;
                    if(counter % 2 == 0) {
                        midi_controller_change(0, control, reversed ? VELOCITY_BACKWARD : VELOCITY_FORWARD);
                    }
                } else {
                    counter--;
                    if(counter % 2 == 0) {
                         midi_controller_change(0, control, reversed ? VELOCITY_FORWARD : VELOCITY_BACKWARD);
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
        int counter = 1;
        int control;
        bool reversed;
};