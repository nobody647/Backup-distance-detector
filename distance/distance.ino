#include <ESP8266WiFi.h>

const char* ssid = "ESP";
const char* pw = "Password";

const int trigger = D2;
const int echo = D1;


WiFiServer server(80);

const bool debugMode = true;  // whether or not to use serial to print debug
                              // info

const bool sleepMode = 1; //1 for deep sleep, 0 for delay


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
    delayMicroseconds(30);
    if (digitalRead(echo) == HIGH) {
        debugLog("wtf???");
        delayMicroseconds(100);
        return getDist();
    }
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger, LOW);

    pinMode(echo, INPUT);
    long duration = pulseIn(echo, HIGH);
    debugLog(String((duration / 2) / 29.1));

    return (duration / 2) / 29.1;  // Distance in cm
}

long avgDist(int times) {  // Returns a smoothed distance given a number of
                           // readings to take
    long result = 0;

    for (int i = 0; i < times; i++) {
        result += getDist();
    }

    return result / times;
}

void sleep(int duration){
  if (sleepMode == 1){
    ESP.deepSleep(duration*1000);
  }else{
    delay(duration);
  }
}

void setup() {
    pinMode(trigger, OUTPUT);
    pinMode(echo, INPUT);
    
    debugLog("Connecting to network: ");
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        debugLog("oof, we're disconnected");
        if (networkOnline()){
            debugLog("Network is online");
            connectToESP();
        }else{
            debugLog("Network not online");
            sleep(500);
            return;
        }

    }
    handle();
    delay(50);
}

void connectToESP() {
    WiFi.begin(ssid, pw);

    while (WiFi.status() != WL_CONNECTED) {  // Waits until connected to network
        delay(50);
        debugLog(".", false);
    }
    debugLog("We did it!");

    // Static ip
    IPAddress ip(192, 168, 1, 50);
    IPAddress gateway(192, 168, 1, 1);
    IPAddress subnet(255, 255, 255, 0);
    WiFi.config(ip, gateway, subnet);

    server.begin();
    debugLog(WiFi.localIP().toString());
}

bool networkOnline() {
    debugLog("Scanning for networks");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);


    int n = WiFi.scanNetworks();  // Number of networks detected

    for (int i = 0; i < n; i++) {
        if (WiFi.SSID(i) == "ESP") {  // Sorta like an array I guess? Idk how to explain it.
                      // Wifi.ssid(i) gets the ssid of the ith network detected
            debugLog("Found!");
            return true;
        }
    }
    return false;
}

void handle() {
    WiFiClient client = server.available();
    if (client) {
        
        debugLog("Incoming connection");

        int timeout = 3000;
        while (!client.available()) {
            delay(1);
            timeout --;

            if (timeout < 0){
                debugLog("Timeout while while waiting for client");
                return;
            }
        }

        long dist = avgDist(5);

        debugLog("Avg distance measured as " + String(dist));
        client.print(String(dist));
    }
}
