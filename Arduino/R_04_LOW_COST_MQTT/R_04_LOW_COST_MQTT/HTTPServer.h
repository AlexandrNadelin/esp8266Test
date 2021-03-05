#ifndef __HTTP_SERVER_H
#define __HTTP_SERVER_H
#include <Arduino.h>
#include "Config.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "MemoryManager.h"
#include "TimeSpan.h"
#include "PinsStateManager.h"
#include "WebSite.h"
#include <ArduinoJson.h>

static ESP8266WebServer* apServerPtr;

class HTTPServer
{
  public:
    HTTPServer(){}
    ~HTTPServer(){}

    void begin(MemoryManager* memoryManager,PinsStateManager* pinsStateManager)
    {
      apServerPtr=&this->apServer;
      
      this->memoryManager=memoryManager;
      this->pinsStateManager = pinsStateManager;
      
      WiFi.mode(WIFI_AP);      
      apIP.fromString(memoryManager->networkProperty.apIP);
      apGateWay.fromString(memoryManager->networkProperty.apGateWay);
      apSubnet.fromString(memoryManager->networkProperty.apSubnet);
      
      #ifdef SERIAL_DEBUG_ENABLED
        Serial.print("Setting soft-AP configuration ... ");
        Serial.println(WiFi.softAPConfig(apIP, apGateWay, apSubnet) ? "Ready" : "Failed!");
      #else      
        bool result = WiFi.softAPConfig(apIP, apGateWay, apSubnet);
      #endif

      #ifdef SERIAL_DEBUG_ENABLED
        Serial.print("Setting soft-AP ... ");
        Serial.println(WiFi.softAP(memoryManager->networkProperty.apSSID,memoryManager->networkProperty.apPassword) ? "Ready" : "Failed!");
      #else
        result = WiFi.softAP(memoryManager->networkProperty.apSSID,memoryManager->networkProperty.apPassword);
      #endif
      
      #ifdef SERIAL_DEBUG_ENABLED
        Serial.print("Soft-AP IP address = ");
        Serial.println(WiFi.softAPIP());
      #endif
      
      //---apServer.on("/R_04.reboot",HTTP_GET, [](){apServer.send(200);});---//
      apServer.on("/",[](){
        //if(!server.authenticate(www_username, www_password))
        //return server.requestAuthentication();//     
        apServerPtr->send(200, "text/html", data_Index_html, sizeof(data_Index_html)); 
      });  
      apServer.on("/Index.html",[](){     
        apServerPtr->send(200, "text/html", data_Index_html, sizeof(data_Index_html)); 
      });     
      apServer.on("/style.css",[](){
        apServerPtr->send(200, "text/css", data_style_css, sizeof(data_style_css)); 
      });
      apServer.on("/PinManager.html",[](){
        apServerPtr->send(200, "text/html", data_PinManager_html, sizeof(data_PinManager_html)); 
      });
      apServer.onNotFound([](){
        apServerPtr->send(404, "text/plain", "404: Not found"); 
      });
      apServer.on("/NetworkParameters.property",HTTP_GET,[](){
        //---reading property---//
        File networkParameters;
        if(!(networkParameters = SPIFFS.open("/NetworkParameters.property", "r")))
        {
          #ifdef SERIAL_DEBUG_ENABLED
          Serial.println("Failed to open NetworkParameters.property to read");
          #endif
          return;
        }

        MemoryManager memoryManager;

        char arraySettings[2000];
        int numberOfBytes = sprintf(arraySettings,"{\"ApSSID\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&networkParameters,&arraySettings[numberOfBytes]);
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\",\"ApPASSWORD\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&networkParameters,&arraySettings[numberOfBytes]);
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\",\"ApIPAddress\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&networkParameters,&arraySettings[numberOfBytes]);
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\",\"ApSubnetMask\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&networkParameters,&arraySettings[numberOfBytes]);
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\",\"ApGateway\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&networkParameters,&arraySettings[numberOfBytes]);
        
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\",\"StSSID\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&networkParameters,&arraySettings[numberOfBytes]);
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\",\"StPASSWORD\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&networkParameters,&arraySettings[numberOfBytes]);
        
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\",\"MQTTServer\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&networkParameters,&arraySettings[numberOfBytes]);
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\",\"MQTTPort\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&networkParameters,&arraySettings[numberOfBytes]);
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\",\"MQTTLogin\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&networkParameters,&arraySettings[numberOfBytes]);
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\",\"MQTTPassword\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&networkParameters,&arraySettings[numberOfBytes]);
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\",\"MQTTClientID\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&networkParameters,&arraySettings[numberOfBytes]);
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\",\"MQTTPublishPeriod\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&networkParameters,&arraySettings[numberOfBytes]);
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\"}");
        arraySettings[numberOfBytes]=0;
 
        networkParameters.close();

        apServerPtr->send(200, "application/json", arraySettings,numberOfBytes);
      });
      
      apServer.on("/NetworkParameters.property",HTTP_POST, [](){
      String data = apServerPtr->arg("plain"); 
      StaticJsonDocument<2000> doc;
      DeserializationError error = deserializeJson(doc, data);

      if(error){
        #ifdef SERIAL_DEBUG_ENABLED
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
        #endif
        return;
      }
      
      File networkParameters;
      if(!(networkParameters = SPIFFS.open("/NetworkParameters.property", "w")))
      {
        #ifdef SERIAL_DEBUG_ENABLED
          Serial.println("Failed to open NetworkParameters.property to write");
        #endif
        return;
      }  
      String ApSSID = doc["ApSSID"];
      String ApPASSWORD = doc["ApPASSWORD"];
      String ApIPAddress = doc["ApIPAddress"];
      String ApSubnetMask = doc["ApSubnetMask"];
      String ApGateway = doc["ApGateway"];
      
      String StSSID = doc["StSSID"];
      String StPASSWORD = doc["StPASSWORD"];
      
      String MQTTServer = doc["MQTTServer"];
      String MQTTPort = doc["MQTTPort"];
      String MQTTLogin = doc["MQTTLogin"];
      String MQTTPassword = doc["MQTTPassword"];
      String MQTTClientID = doc["MQTTClientID"];
      String MQTTPublishPeriod = doc["MQTTPublishPeriod"]; 
      
      networkParameters.println(ApSSID);
      networkParameters.println(ApPASSWORD);
      networkParameters.println(ApIPAddress);
      networkParameters.println(ApSubnetMask);
      networkParameters.println(ApGateway);
      
      networkParameters.println(StSSID);
      networkParameters.println(StPASSWORD);
      
      networkParameters.println(MQTTServer);
      networkParameters.println(MQTTPort);
      networkParameters.println(MQTTLogin);
      networkParameters.println(MQTTPassword);
      networkParameters.println(MQTTClientID);
      networkParameters.println(MQTTPublishPeriod); 

      networkParameters.close();
    
      apServerPtr->send(200);
      });
      //----------------------------------------------------------------------//
      apServer.begin(80);

      #ifdef SERIAL_DEBUG_ENABLED
      Serial.println("HTTP server started");
      Serial.println();
      #endif
    }
    
    void loop()
    {
      this->apServer.handleClient();
    }

  private:
    ESP8266WebServer apServer;
    IPAddress apIP;
    IPAddress apGateWay;
    IPAddress apSubnet;
    MemoryManager* memoryManager;
    PinsStateManager* pinsStateManager;
};

#endif
