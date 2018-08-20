/**
 * 
 * 
 * all of this shit is depracated or whaterver
 * 
 * i redid this in micropython because im lazy and python is easy
 * 
 * 
 * 
 * 
**/






#include <ESP8266WiFi.h>

IPAddress local(192, 168, 1, 2);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() {
    Serial.begin(115200);
    Serial.println("yeet");
    WiFi.softAPdisconnect(true);

    WiFi.softAPConfig(local, gateway, subnet);
    boolean result = WiFi.softAP("ESP", "Password");

    if (result) {
        Serial.println("ITS WORKING!");
    } else {
        Serial.println("lol nope");
    }
    Serial.println(WiFi.softAPIP());
}

void loop() {
    if (Serial.available() > 0) {
        Serial.readString();
        Serial.println("gotcha");
        Serial.printf("Stations connected = %d\n", WiFi.softAPgetStationNum());

        WiFiClient client;
        IPAddress server(192, 168, 1, 50);
        client.connect(server, 80);
        Serial.println("connected and waiting");
        client.print("gimmu ur data boi");
        Serial.println("sent request");
        unsigned long timeout = millis();
        while (client.available() == 0) {
            if (millis() - timeout > 5000) {
                Serial.println(">>> Client Timeout !");
                client.stop();
                return;
            }
        }

        // Read all the lines of the reply from server and print them to Serial
        while (client.available()) {
            String line = client.readString();
            Serial.print(line);
        }
        
    }
    delay(50);
}