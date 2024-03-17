#include <Arduino.h>
#include <SerialCommunication.h>
#include "ardumidi/ardumidi.h"

#define MAX_VALUE 1023
#define MIDI_CHANNEL 0

class Potentiometer {

    public:

        boolean isLogarithmic = false;
        boolean isSwapped = false;
        boolean reverseLogarithmic = false;

        void connect(int pin) {
            pinMode(pin, INPUT);
            input_pin = pin;
        }

        void setControllerChange(int cc) {
            this->cc = cc;
        }

        void blockOnRelease(bool* addr) {
            block = addr;
        }

        void check() {
            int value = !isSwapped && !reverseLogarithmic ? analogRead(input_pin) : MAX_VALUE - analogRead(input_pin);
            if(abs(lastState - value) > 3) {
                int normalized = isLogarithmic ? MAX_VALUE - pow(MAX_VALUE - value, 3) / MAX_VALUE / MAX_VALUE : value;

                if(reverseLogarithmic) {
                    normalized = pow(normalized, 3) / MAX_VALUE / MAX_VALUE;
                }

                int velocity = map(normalized, 0, 1023, 0, 127);
                midi_controller_change(MIDI_CHANNEL, cc, velocity);

                *block = true;
                lastState = value;
            }
        }

    private:
        int input_pin;
        int lastState;
        int cc;
        bool *condition = NULL;
        bool *block;
};