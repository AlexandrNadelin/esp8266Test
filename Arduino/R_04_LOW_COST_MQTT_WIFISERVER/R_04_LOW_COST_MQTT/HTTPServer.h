#ifndef __HTTP_SERVER_H
#define __HTTP_SERVER_H
#include <Arduino.h>
#include "Config.h"
#include <ESP8266WiFi.h>
/*#include <ESP8266WebServer.h>*/
#include <ArduinoJson.h>
#include "MemoryManager.h"
#include "TimeSpan.h"
#include "PinsStateManager.h"
#include "WebSite.h"
#include <ArduinoJson.h>
#include "Base64.h"
#include "ASN_1.h"

/*static WiFiServer* apServerPtr;
static PinsStateManager* pinsStateManagerHTTPServerPtr;*/

#define RECEIVE_ARRAY_SIZE (uint16_t)1600
char receiveArray[RECEIVE_ARRAY_SIZE];
uint32_t receiveDataCounter=0;

WiFiServer server(80);

class HTTPServer
{
  public:
    HTTPServer(){}
    ~HTTPServer(){}

    void begin(MemoryManager* memoryManager,PinsStateManager* pinsStateManager)
    {
      /*apServerPtr=&this->apServer;
      pinsStateManagerHTTPServerPtr = pinsStateManager;*/
      
      this->memoryManager=memoryManager;
      this->pinsStateManager = pinsStateManager;

      const char* ssid = "TP-LINK_453D2A";
      const char* password = "36216703";
      WiFi.mode(WIFI_STA);  
      WiFi.begin(ssid, password);
      Serial.println("");

      // ждем соединения:
      while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      }
      Serial.println("");
      Serial.print("Connected to ");  //  "Подключились к "
      Serial.println(ssid);
      Serial.print("IP address: ");  //  "IP-адрес: "
      Serial.println(WiFi.localIP());
      
      server.begin();//(80);

      #ifdef SERIAL_DEBUG_ENABLED
      Serial.println("HTTP server started");
      Serial.println();
      #endif
    }
    
    void loop()
    {
      WiFiClient client = server.available();
      // wait for a client (web browser) to connect
      if (client)
      {
        Serial.println("\n[Client connected]");
        receiveDataCounter=0;
        if(client.connected())
        {
          while(client.available())
          {
            receiveArray[receiveDataCounter++]=client.read();
            if(receiveDataCounter==RECEIVE_ARRAY_SIZE)
            {
              receiveDataCounter=0;
              Serial.println("RECEIVE_ARRAY_SIZE exceeded");
              break;
            }            
          }         
        }

        for(int i = 0;i<receiveDataCounter;i++)
        {
          Serial.print(receiveArray[i]);
        }

        // close the connection:
        client.stop();
        Serial.println("[Client disconnected]");
       }
    }

  private:
    /*WiFiServer apServer(80);
    IPAddress apIP;
    IPAddress apGateWay;
    IPAddress apSubnet;*/
    MemoryManager* memoryManager;
    PinsStateManager* pinsStateManager;
};

#endif
