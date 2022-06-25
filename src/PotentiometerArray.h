#include "Arduino.h"
#include "SerialCommunication.h"
#include "ShiftRegister.h"

class PotentiometerArray {

    public:

        void setMainConnection(int universalPin, ShiftRegister shiftRegister) {
            pinMode(universalPin, INPUT);
            this->universalPin = universalPin;
            this->shiftRegister = shiftRegister;
        }

        void connectPotenitometer(int vccPin, String serialHeader) {
            shiftRegister.write(vccPin, LOW);
            
            int count = getCount();

            outPins = new int[count + 1];
            outPins[count] = vccPin;

            lastValues = new int[count + 1];
            lastValues[count] = 0;

            headers = new String[count + 1];
            headers[count] = serialHeader;
        }

        void check() {
            for(int i = 0; i < getCount(); i++) {
                int outPin = outPins[(i + 1) % getCount()];

                shiftRegister.write(outPin, HIGH);
                int value = analogRead(outPin);

                if(abs(lastValues[i] - value) > 3) {
                    SerialCommunication::sendCommand(headers[i], value);
                    lastValues[i] = value;
                }

                delay(1);

                 shiftRegister.write(outPins[i], HIGH);
            }
        }

    private:

        ShiftRegister shiftRegister;
        String* headers;
        int* outPins;
        int* lastValues;
        int universalPin;

        int getCount() {
            return sizeof(outPins) / sizeof(outPins[0]);
        }
        
};