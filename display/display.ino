#include <ESP8266WiFi.h>

char* APName = "test";

void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Starting AP named %f", APName);
  WiFi.softAP(APName);

}


void loop()
{

}