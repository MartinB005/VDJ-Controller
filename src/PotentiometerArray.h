#include "Arduino.h"
#include "SerialCommunication.h"
#include "ShiftRegister.h"

#define COUNT 4
#define MAX_VALUE 1023

typedef void(*idleFunc)(void);

class PotentiometerArray {

    public:

        void setMainConnection(int universalPin, ShiftRegister shiftRegister) {
            pinMode(universalPin, INPUT);
            this->universalPin = universalPin;
            this->shiftRegister = shiftRegister;
        }

        void connectPotenitometer(int vccPin, int cc, boolean swap) {
            shiftRegister.write(vccPin, LOW);

            outPins[count] = vccPin;
            swaped[count] = swap;

            lastValues[count] = 0;

            controls[count] = cc;
            count++;
        }

        void setIdleFunction(idleFunc func) {
            this->func = func;
        }

        void check() {

            for(int i = 0; i < COUNT; i++) {
                int outPin = outPins[i];
                outPin = outPins[i];

                shiftRegister.write(outPin, HIGH);
                
                int millisValue = millis();
                while(millis() - millisValue < 20) {
                    //func();
                }

                readData(outPin, i);
            }
        }

    private:

        ShiftRegister shiftRegister;
        int controls[COUNT];
        int outPins[COUNT];
        int lastValues[COUNT];
        int universalPin;
        int maxCount;
        int count;
        long lastMillisValue = __LONG_MAX__;

        boolean next = true;
        boolean swaped[COUNT];
        idleFunc func;

        void readData(int outPin, int which) {

                int value = analogRead(universalPin);

                if(swaped[which]) {
                    value = MAX_VALUE - value;
                }

                if(abs(lastValues[which] - value) > 5) {
                    int velocity = map(value, 0, MAX_VALUE, 0, 127);
                    midi_controller_change(MIDI_CHANNEL, controls[which], velocity);
                    lastValues[which] = value;
                }

                shiftRegister.write(outPin, LOW);

                which = which < 3 ? which + 1 : 0;
                next = true;

                shiftRegister.release();
        }
        
};