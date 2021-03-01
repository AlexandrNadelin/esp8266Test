#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "Config.h"
#include "MemoryManager.h"
#include "MQTTConnection.h"
#include "TimeSpan.h"
#include "PinsStateManager.h"
#include "HTTPServer.h"

MemoryManager memoryManager;
PinsStateManager pinsStateManager;
Ticker pinsStateManagerLoopTicker;
MQTTConnector mqttConnector;
HTTPServer httpServer;

void setup() {
  #ifdef SERIAL_DEBUG_ENABLED
  Serial.begin(115200);
  Serial.println();
  #endif

  memoryManager.begin();
  pinsStateManager.begin();

  pinsStateManagerLoopTicker.attach(1.0,[](){pinsStateManager.loop();}); 
  
  if(pinsStateManager.getDINModeState()) mqttConnector.begin(&memoryManager,&pinsStateManager);
  else httpServer.begin(&memoryManager,&pinsStateManager);
}

void loop() {
  if(pinsStateManager.getDINModeState()) mqttConnector.loop();
  else httpServer.loop();
}
