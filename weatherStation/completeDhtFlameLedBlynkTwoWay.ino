#define BLYNK_TEMPLATE_ID "TMPL3v5NANQqA"
#define BLYNK_TEMPLATE_NAME "PROJECT"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"
#define BLYNK_PRINT Serial
#define APP_DEBUG

#include "BlynkEdgent.h"

#include <DHT.h>
#include <DHT_U.h>

#define TYPE DHT11
#define pinDht D6
DHT sensorDht(pinDht, TYPE);

#define mainLed D2
#define pinFlame D8

BlynkTimer timer;

double tmp, humid;
int flame = -1;
int dhtId, flameId;  //for timer Ids...

void setup()
{
  Serial.begin(115200);
  delay(100);

  pinMode(pinFlame, INPUT);
  pinMode(mainLed, OUTPUT);

  sensorDht.begin();
  Serial.println(sensorDht.read());
  delay(100);
  
  dhtId = timer.setInterval(5000L, readDht);
  flameId = timer.setInterval(2000L, readFlame);
  BlynkEdgent.begin();
}

void readDht()
{
  if(isnan(tmp) || isnan(humid))
  {
    Blynk.virtualWrite(V1, 0);
    Blynk.virtualWrite(V2, 0);
  }
  
  tmp = sensorDht.readTemperature();
  Serial.println(tmp);
  Blynk.virtualWrite(V1, tmp);
  delay(100);
  humid = sensorDht.readHumidity();
  Serial.println(humid);
  Blynk.virtualWrite(V2, humid);
  delay(100);
}

void readFlame()
{
  flame = digitalRead(pinFlame);    
  Serial.println(flame);
  if(flame == 1)
  {
    Blynk.virtualWrite(V3, 0);       //flame sensor is working in revesre logic. 
  }                                  //If flame it is giving output -> 0 else 1... reversed logic
  else
  {
    Blynk.virtualWrite(V3, 1);
  }
  
  delay(100);
}

BLYNK_CONNECTED()
{
  Blynk.syncAll();
}

BLYNK_WRITE(V0)
{
  int push = param.asInt();
  if(push == 1)
  {
    digitalWrite(mainLed, HIGH);
  }
  else
  {
    digitalWrite(mainLed, LOW);
  }
}

void loop() 
{
  BlynkEdgent.run();
  timer.run();
}

