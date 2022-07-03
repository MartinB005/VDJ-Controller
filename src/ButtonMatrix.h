#include <Arduino.h>
#include <SerialCommunication.h>
#include <ShiftRegister.h>

#define COLUMNS 4
#define ROWS 4


typedef void(*idleFunc)(void);

class ButtonMatrix {

    public:

        void connect(ShiftRegister shiftRegister, int columnPins[], int rowPins[]) {
            
            this->shiftRegister = shiftRegister;

            for(int i = 0; i < ROWS; i++) {
                this->rowPins[i] = rowPins[i];
                pinMode(rowPins[i], INPUT_PULLUP);
                Serial.println(rowPins[i]);
            }

            for(int j = 0; j < COLUMNS; j++) {
                this->columnPins[j] = columnPins[j];
                shiftRegister.write(columnPins[j], HIGH);
            }
        }

        void setButtonHeader(int column, int row, String header) {
            headers[column][row] = header;
        }

        void setIdleFunction(idleFunc func) {
            this->func = func;
        }

        void check() {

            for(int column = 0; column < COLUMNS; column++) {

                shiftRegister.write(columnPins[column], LOW);

                int millisValue = millis();
                while(millis() - millisValue < 5) {
                    func();
                }

                readData(column);
            }
        }

    private:

        ShiftRegister shiftRegister;
        String headers[COLUMNS][ROWS];
        int columnPins[COLUMNS];
        int rowPins[ROWS];

        int pressed[COLUMNS][ROWS];
        long lastMillisValue = __LONG_MAX__;
        boolean next = true;
        idleFunc func;

        
        void readData(int column) {

            for(int row = 0; row < ROWS; row++) {

                if(digitalRead(rowPins[row]) == LOW) {

                    if(!pressed[column][row]) {
                        SerialCommunication::sendCommand(headers[column][row], 1);
                        pressed[column][row] = true;
                    }

                } else {
                    pressed[column][row] = false;
                }
            }

            shiftRegister.write(columnPins[column], HIGH);

            column = column < COUNT - 1 ? column + 1 : 0;
            next = true;

        }
};