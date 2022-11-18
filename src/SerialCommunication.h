#include <ArduinoJson.h>

#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

class SerialCommunication {

    public:

        static void sendCommand(String header, int value) {
            StaticJsonDocument<200> doc;
            doc["header"] = header;
            doc["value"] = value;
            String jsonString;
            serializeJson(doc, Serial);
            Serial.print("\n");
        }

        static void sendESC() {
            StaticJsonDocument<200> doc;
            doc["header"] = "ESC";
            doc["value"] = 1;
            String jsonString;
            serializeJson(doc, Serial);
            Serial.print("\n");
        }
};

#endif // SERIALCOMMUNICATION_H