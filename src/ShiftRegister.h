#include "Arduino.h"
#include <SerialCommunication.h>

#define OUTPUTS_COUNT 8

#ifndef SHIFTREGISTER_H
#define SHIFTREGISTER_H

class ShiftRegister {

    public:

        void connect(int dataPin, int clockPin, int latchPin) {
            this->dataPin = dataPin;
            this->clockPin = clockPin;
            this->latchPin = latchPin;

            pinMode(latchPin, OUTPUT);
            pinMode(clockPin, OUTPUT);
            pinMode(dataPin, OUTPUT);
        }

        void setSerialHeader(String header) {
            this->header = header;
        }

        void write(int pin, int value) {
            currentNumber[OUTPUTS_COUNT - pin - 1] = value == HIGH ? '1' : '0';

            digitalWrite(latchPin, LOW);
            
            shiftOut(dataPin, clockPin, MSBFIRST, binary_to_int(currentNumber));
 
            digitalWrite(latchPin, HIGH);


        }

        void wait() {
            busy = true;
        }

        boolean isBusy() {
            return busy;
        }

        void release() {
            busy = false;
        }

    private:
        int latchPin;
        int clockPin;
        int dataPin;
        char* currentNumber = "00000000";
        boolean busy = false;
        String header;

        long binary_to_int(char *binary_string) {
            long total = 0;
            while (*binary_string)
            {
            total *= 2;
            if (*binary_string++ == '1') total += 1;
            }
    
            return total;
        }

};

#endif //SHIFTREGISTER_H