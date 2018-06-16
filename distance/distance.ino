#include <ESP8266WiFi.h>

const char* ssid = "Barker";
const char* pw = "BarkerWifi";

const int trigger = D0;
const int echo = D1;

WiFiServer server(80);

const bool debugMode = true;  // whether or not to use serial to print debug
                              // info

void debugLog(String message, bool newline = true) {
    if (!debugMode) {
        return;
    }
    if (!Serial) {
        Serial.begin(115200);
        while (!Serial) {
            // Waits for serial to connect
        }
    }

    if (newline) {
        Serial.println(message);
    } else {
        Serial.print(message);
    }
}

long getDist() {
    digitalWrite(trigger, LOW);
    delayMicroseconds(5);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);

    pinMode(echo, INPUT);
    long duration = pulseIn(echo, HIGH);

    return (duration / 2) / 29.1;  // Distance in cm
}

void setup() {
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    debugLog("Connecting to network: ", true);

    WiFi.begin(ssid, pw);

    while (WiFi.status() != WL_CONNECTED) {  // Waits until
        delay(50);
        debugLog(".", false);
    }
    debugLog("We did it!");
    server.begin();
    debugLog(WiFi.localIP().toString());
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        debugLog("Incoming connection from "+client.localIP().toString());

        while (!client.available()) {
            delay(1);
        }

        long dist = getDist();

        debugLog("Distance measured as "+String(dist));
        client.print(String(dist));
    }
}