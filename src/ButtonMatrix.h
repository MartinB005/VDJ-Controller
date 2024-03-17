#include <Arduino.h>
#include <SerialCommunication.h>
#include <ShiftRegister.h>

#define COLUMNS 4
#define ROWS 4

#define CUE_LEFT 0x15
#define PLAY_LEFT 0x16
#define SYNC_LEFT 0x17

#define CUE_RIGHT 0x18
#define PLAY_RIGHT 0x19
#define SYNC_RIGHT 0x1A

#define EFFECT_LEFT 0x1B
#define EFFECT_RIGHT 0x1C

#define PAD_1_LEFT 0x1D
#define PAD_2_LEFT 0x1E
#define PAD_3_LEFT 0x1F
#define PAD_4_LEFT 0x20

#define PAD_1_RIGHT 0x21
#define PAD_2_RIGHT 0x22
#define PAD_3_RIGHT 0x23
#define PAD_4_RIGHT 0x24

const int controls[COLUMNS][ROWS] = {{PAD_1_LEFT, SYNC_LEFT, PAD_1_RIGHT, EFFECT_RIGHT},
                                     {PAD_2_LEFT, CUE_LEFT, PAD_2_RIGHT, SYNC_RIGHT},
                                     {PAD_3_LEFT, EFFECT_LEFT, PAD_3_RIGHT, CUE_RIGHT},
                                     {PAD_4_LEFT, PLAY_LEFT, PAD_4_RIGHT, PLAY_RIGHT}};

typedef void (*idleFunc)(void);

class ButtonMatrix
{

public:
    void init(ShiftRegister shiftRegister, int columnPins[], int rowPins[])
    {

        this->shiftRegister = shiftRegister;

        for (int i = 0; i < ROWS; i++)
        {
            this->rowPins[i] = rowPins[i];
            pinMode(rowPins[i], INPUT_PULLUP);
            Serial.println(rowPins[i]);
        }

        for (int j = 0; j < COLUMNS; j++)
        {
            this->columnPins[j] = columnPins[j];
            shiftRegister.write(columnPins[j], HIGH);
        }
    }

    void setIdleFunction(idleFunc func)
    {
        this->func = func;
    }

    void check()
    {

        for (int column = 0; column < COLUMNS; column++)
        {

            shiftRegister.write(columnPins[column], LOW);

            int millisValue = millis();
            while (millis() - millisValue < 5)
            {
                func();
            }

            readData(column);
        }
    }

    bool *getBlockControl()
    {
        return &blockButtons;
    }

private:
    ShiftRegister shiftRegister;
    boolean pressed[COLUMNS][ROWS];
    int columnPins[COLUMNS];
    int rowPins[ROWS];
    long lastMillisValue = __LONG_MAX__;
    boolean next = true;
    boolean blockButtons;
    idleFunc func;

    void readData(int column)
    {

        for (int row = 0; row < ROWS; row++)
        {

            if (digitalRead(rowPins[row]) == LOW)
            {

                if (!pressed[column][row])
                {
                    midi_note_on(1, controls[column][row], 127);
                    blockButtons = false;
                    pressed[column][row] = true;
                }
            }
            else
            {
                if (pressed[column][row])
                {
                    midi_note_on(1, controls[column][row], 0);
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