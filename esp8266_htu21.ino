#include <Wire.h>
#include <Sodaq_SHT2x.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define CONTROL_PIN D7

char auth[] = "***********************";
char ssid[] = "7SkyHome";
char pass[] = "**********************";
 
void setup()
{
Serial.begin(9600);
Blynk.begin(auth, ssid, pass);
Wire.begin();
pinMode(CONTROL_PIN, OUTPUT);
}
 
void loop()
{
float hum = SHT2x.GetHumidity();
float temp = SHT2x.GetTemperature();
float dew_point = SHT2x.GetDewPoint();
int humidifierState;

Serial.println("Humidity(%RH): " + String(hum));
Serial.println(" Temperature(C): " + String(temp));
Serial.println(" Dewpoint(C): " + String(dew_point));

if(hum <= 30)
    humidifierState = 1;
else
    humidifierState = 0;

if(humidifierState == 1)
  digitalWrite(CONTROL_PIN, HIGH);
else
  digitalWrite(CONTROL_PIN, LOW);

Blynk.run();
Blynk.virtualWrite(V0, hum);
Blynk.virtualWrite(V1, temp);
Blynk.virtualWrite(V2, dew_point);
Blynk.virtualWrite(V3, dew_point);

delay(1000);
}
