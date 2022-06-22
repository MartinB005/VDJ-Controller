#include "Arduino.h"
#include <SerialCommunication.h>

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
            currentNumber[pin] = value == HIGH ? '1' : '0';

            digitalWrite(latchPin, LOW);
 
            shiftOut(dataPin, clockPin, MSBFIRST, binary_to_int(currentNumber));
 
            digitalWrite(latchPin, HIGH);
        }

    private:
        int latchPin;
        int clockPin;
        int dataPin;
        char* currentNumber;
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