#include <Arduino.h>
#include <SerialCommunication.h>
#include <ShiftRegister.h>

#define COLUMNS 4
#define ROWS 4

#define SEND_ON_KEYUP 1
#define SEND_ON_KEYDOWN 0

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

        void setButtonHeader(int column, int row, String header, boolean whenSend) {
            headers[column][row] = header;
            detectKeyUp[column][row] = whenSend;
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

        bool* getBlockControl() {
            return &blockButtons;
        }

        bool* getButtonAddress(String header) {
            for(int i = 0; i < COLUMNS; i++) {
                
                for(int j = 0; j < ROWS; j++) {
                    if(header.equals(headers[i][j]))
                        return &pressed[i][j];
                }
            }

            return NULL;
        }

    private:

        ShiftRegister shiftRegister;
        String headers[COLUMNS][ROWS];
        boolean pressed[COLUMNS][ROWS];
        boolean detectKeyUp[COLUMNS][ROWS];
        int columnPins[COLUMNS];
        int rowPins[ROWS];
        long lastMillisValue = __LONG_MAX__;
        boolean next = true;
        boolean blockButtons;
        idleFunc func;

        
        void readData(int column) {

            for(int row = 0; row < ROWS; row++) {

                if(digitalRead(rowPins[row]) == LOW) {

                    if(!pressed[column][row]) {
                        if(!detectKeyUp[column][row]) SerialCommunication::sendCommand(headers[column][row], 1);
                        blockButtons = false;
                        pressed[column][row] = true;
                    }

                } else {
                    if(pressed[column][row] && detectKeyUp[column][row]) {
                        if(!blockButtons) SerialCommunication::sendCommand(headers[column][row], 1);
                        else SerialCommunication::sendESC();
                        blockButtons = false;
                    }
                    pressed[column][row] = false;
                }
            }

            shiftRegister.write(columnPins[column], HIGH);

            column = column < COUNT - 1 ? column + 1 : 0;
            next = true;

        }
};