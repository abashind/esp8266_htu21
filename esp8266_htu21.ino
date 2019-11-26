#include <Wire.h>
#include <Sodaq_SHT2x.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define CONTROL_PIN 13

char auth[] = "***";
char ssid[] = "7SkyHome";
char pass[] = "***";

float hum = 0;
float temp = 0;
float dew_point = 0;
int manual_mode = 1;
int humidifier_state = 0;

BlynkTimer timer;

BLYNK_WRITE(V3)
{
  humidifier_state = param.asInt();
}

BLYNK_WRITE(V4)
{
  manual_mode = param.asInt();
}

void setup()
{
Serial.begin(9600);
Blynk.begin(auth, ssid, pass);
Wire.begin();
pinMode(CONTROL_PIN, OUTPUT);
timer.setInterval(1000L, do_it);
}

void do_it()
{
hum = SHT2x.GetHumidity();
temp = SHT2x.GetTemperature();
dew_point = SHT2x.GetDewPoint();

Serial.println(" Humidity(%RH): " + String(hum));
Serial.println(" Temperature(C): " + String(temp));
Serial.println(" Dewpoint(C): " + String(dew_point));
Serial.println(" Manual mode: " + String(manual_mode));
Serial.println(" Humidifier state: " + String(humidifier_state));

if(humidifier_state == 1)
  digitalWrite(CONTROL_PIN, HIGH);
else
  digitalWrite(CONTROL_PIN, LOW);

if (manual_mode != 1)
{
  if(hum <= 30)
    humidifier_state = 1;
  else
    humidifier_state = 0;
  Blynk.virtualWrite(V3, humidifier_state);
}
Blynk.virtualWrite(V0, hum);
Blynk.virtualWrite(V1, temp);
Blynk.virtualWrite(V2, dew_point);
}
 
void loop()
{
Blynk.run();
timer.run();
}
