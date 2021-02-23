#include <ESP8266WiFi.h>
#include "Config.h"
#include "MemoryManager.h"
#include "MQTTConnection.h"
#include "TimeSpan.h"

MemoryManager memoryManager;
MQTTConnector mqttConnector;

//unsigned long previousPublishTime=0;

void setup() {
  #ifdef SERIAL_DEBUG_ENABLED
  Serial.begin(115200);
  Serial.println();
  #endif

  memoryManager.begin();
  mqttConnector.begin(&memoryManager);
}

void loop() {
  mqttConnector.loop();

  /*unsigned long currentTime=millis();
  unsigned long timeSpan=calculateTimeSpan(currentTime, previousPublishTime);
  if(timeSpan>2000)
  {
      Serial.println("Time ellapsed");//mqtt.publish("base/state/adc", "hello mqtts");
      previousPublishTime=currentTime;
  }*/
}
