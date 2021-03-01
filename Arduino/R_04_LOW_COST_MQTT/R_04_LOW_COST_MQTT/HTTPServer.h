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
#include "FsData.h"
#include "FsFile.h"

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
        struct fs_file file; 
        fs_open(&file,"Index.html");
        apServerPtr->send(200, "text/html", file.data, file.len); 
      });
      apServer.on("/style.css",[](){
        struct fs_file file;
        fs_open(&file,"style.css");
        apServerPtr->send(200, "text/css",file.data, file.len); 
      });
      apServer.on("/favicon.ico",[](){
        struct fs_file file;
        fs_open(&file,"favicon.ico");
        apServerPtr->send(200, "image/x-icon", file.data, file.len); 
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
