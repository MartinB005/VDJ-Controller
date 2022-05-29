#include <Arduino.h>
#include <SerialCommunication.h>

class Button {

    public:

        void connect(int pin) {
            pinMode(pin, INPUT_PULLUP);
            button_pin = pin;
        }

        void setAction(String header, int value) {
            this->header = header;
            this->value = value;
        }

        void check() {
            if(digitalRead(button_pin) == LOW) {
                if(lastState == HIGH) {
                    SerialCommunication::sendCommand(header, value);
                }
                lastState = LOW;
            }
            else lastState = HIGH;
        }

    private:
        int button_pin;
        int value;
        int lastState = HIGH;
        String header;
};