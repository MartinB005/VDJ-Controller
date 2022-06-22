#include "Arduino.h"
#include "SerialCommunication.h"

class PotentiometerArray {

    public:

        void setMainConnection(int universalPin) {
            pinMode(universalPin, INPUT);
            this->universalPin = universalPin;
        }

        void connectPotenitometer(int vccPin, String serialHeader) {
            pinMode(vccPin, OUTPUT);
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

                digitalWrite(outPin, HIGH);
                int value = analogRead(outPin);
                
                if(abs(lastValues[i] - value) > 3) {
                    SerialCommunication::sendCommand(headers[i], value);
                    lastValues[i] = value;
                }

                digitalWrite(outPins[i], LOW);
            }
        }

    private:

        String* headers;
        int* outPins;
        int* lastValues;
        int universalPin;

        int getCount() {
            return sizeof(outPins) / sizeof(outPins[0]);
        }
        
};