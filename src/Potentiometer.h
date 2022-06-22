#include <Arduino.h>
#include <SerialCommunication.h>

#define MAX_VALUE 1023

class Potentiometer {

    public:

        boolean isLogarithmic = false;
        boolean isSwapped = false;
        boolean reverseLogarithmic = false;

        void connect(int pin) {
            pinMode(pin, INPUT);
            input_pin = pin;
        }

        void setSerialHeader(String header) {
            this->header = header;
        }

        void check() {
            int value = !isSwapped && !reverseLogarithmic ? analogRead(input_pin) : MAX_VALUE - analogRead(input_pin);
            if(abs(lastState - value) > 3) {
                int normalized = isLogarithmic ? MAX_VALUE - pow(MAX_VALUE - value, 3) / MAX_VALUE / MAX_VALUE : value;

                if(reverseLogarithmic) {
                    normalized = pow(normalized, 3) / MAX_VALUE / MAX_VALUE;
                }
                
                SerialCommunication::sendCommand(header, normalized);
                lastState = value;
            }
        }

    private:
        int input_pin;
        int lastState;
        String header;
};