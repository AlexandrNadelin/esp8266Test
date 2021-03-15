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
#include "Base64.h"
#include "ASN_1.h"

static ESP8266WebServer* apServerPtr;
static PinsStateManager* pinsStateManagerHTTPServerPtr;

class HTTPServer
{
  public:
  
    /*static PinsStateManager* pinsStateManagerPtr;*/
    HTTPServer(){}
    ~HTTPServer(){}

    void begin(MemoryManager* memoryManager,PinsStateManager* pinsStateManager)
    {
      apServerPtr=&this->apServer;

      pinsStateManagerHTTPServerPtr = pinsStateManager;
      
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
      
      /*WiFi.mode(WIFI_AP);      
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
      #endif*/
      
      //---apServer.on("/R_04.reboot",HTTP_GET, [](){apServer.send(200);});---//
      apServer.on("/",[](){
        //if(!server.authenticate(www_username, www_password))
        //return server.requestAuthentication();//     
        apServerPtr->send(200, "text/html", data_Index_html, size_Index_html); 
      });  
      apServer.on("/Index.html",[](){     
        apServerPtr->send(200, "text/html", data_Index_html, size_Index_html); 
      });     
      apServer.on("/style.css",[](){
        apServerPtr->send(200, "text/css", data_style_css, size_style_css); 
      });
      apServer.on("/PinManager.html",[](){
        apServerPtr->send(200, "text/html", data_PinManager_html, size_PinManager_html); 
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

        char arraySettings[1000];
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
      StaticJsonDocument<1000> doc;
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
      
      apServer.on("/AINCoefficient.property",HTTP_GET,[](){
        //---reading property---//
        File ainCoefficient;
        if(!(ainCoefficient = SPIFFS.open("/AINCoefficient.property", "r")))
        {
          #ifdef SERIAL_DEBUG_ENABLED
          Serial.println("Failed to open AINCoefficient.property to read");
          #endif
          return;
        }

        MemoryManager memoryManager;

        char arraySettings[128];
        int numberOfBytes = sprintf(arraySettings,"{\"AINCoefficient\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&ainCoefficient,&arraySettings[numberOfBytes]);
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\"}");
        arraySettings[numberOfBytes]=0;
 
        ainCoefficient.close();

        apServerPtr->send(200, "application/json", arraySettings,numberOfBytes);
      });
      
      apServer.on("/AINCoefficient.property",HTTP_POST, [](){
      String data = apServerPtr->arg("plain"); 
      StaticJsonDocument<128> doc;
      DeserializationError error = deserializeJson(doc, data);

      if(error){
        #ifdef SERIAL_DEBUG_ENABLED
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
        #endif
        return;
      }
      
      File ainCoefficient;
      if(!(ainCoefficient = SPIFFS.open("/AINCoefficient.property", "w")))
      {
        #ifdef SERIAL_DEBUG_ENABLED
          Serial.println("Failed to open AINCoefficient.property to write");
        #endif
        return;
      }  
      String AINCoefficient = doc["AINCoefficient"];
      
      ainCoefficient.print(AINCoefficient);

      ainCoefficient.close();
    
      apServerPtr->send(200);
      });
      
      apServer.on("/DOUTStateFormula.property",HTTP_GET,[](){
        File doutStateFormula;
        if(!(doutStateFormula = SPIFFS.open("/DOUTStateFormula.property", "r")))
        {
          #ifdef SERIAL_DEBUG_ENABLED
          Serial.println("Failed to open DOUTStateFormula.property to read");
          #endif
          return;
        }

        MemoryManager memoryManager;

        char arraySettings[128];
        int numberOfBytes = sprintf(arraySettings,"{\"DOUTStateFormula\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&doutStateFormula,&arraySettings[numberOfBytes]);
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\"}");
        arraySettings[numberOfBytes]=0;
 
        doutStateFormula.close();

        apServerPtr->send(200, "application/json", arraySettings,numberOfBytes);
      });
      
      apServer.on("/DOUTStateFormula.property",HTTP_POST, [](){
      String data = apServerPtr->arg("plain"); 
      StaticJsonDocument<128> doc;
      DeserializationError error = deserializeJson(doc, data);

      if(error){
        #ifdef SERIAL_DEBUG_ENABLED
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.f_str());
        #endif
        return;
      }
      
      File doutStateFormula;
      if(!(doutStateFormula = SPIFFS.open("/DOUTStateFormula.property", "w")))
      {
        #ifdef SERIAL_DEBUG_ENABLED
          Serial.println("Failed to open DOUTStateFormula.property to write");
        #endif
        return;
      }  
      String DOUTStateFormula = doc["DOUTStateFormula"];
      
      doutStateFormula.print(DOUTStateFormula);

      doutStateFormula.close();
    
      apServerPtr->send(200);
      });

      apServer.on("/Parameters",HTTP_GET,[](){

        char arraySettings[128];
        int numberOfBytes = sprintf(arraySettings,"{\"A_IN\":\"%f\",\"D_IN_1\":\"%u\",\"D_IN_2\":\"%u\",\"D_OUT_1\":\"%u\"}",pinsStateManagerHTTPServerPtr->getAINState(),pinsStateManagerHTTPServerPtr->getDIN1State(),pinsStateManagerHTTPServerPtr->getDIN2State(),pinsStateManagerHTTPServerPtr->getDOUT1State());
        arraySettings[numberOfBytes]=0;

        apServerPtr->send(200, "application/json", arraySettings,numberOfBytes);
      });

      apServer.on("/pemCA.cer",HTTP_POST, [](){
      String data = apServerPtr->arg("plain");
      File pemCA_cer;
      
      if(!(pemCA_cer = SPIFFS.open("/pemCA.cer", "w")))
      {
        #ifdef SERIAL_DEBUG_ENABLED
          Serial.println("Failed to open pemCA.cer to write");
        #endif
        return;
      }  
      
      pemCA_cer.print(data);

      pemCA_cer.close();
    
      apServerPtr->send(200);
      });

      apServer.on("/PemCAPeriod",HTTP_GET,[](){      
        /*char arraySettings[100];
        int numberOfBytes = sprintf(arraySettings,"{\"Period\":\"%s\"}","fucking period");
        arraySettings[numberOfBytes]=0;*/
        File penCA;
        if(!(penCA = SPIFFS.open("/pemCA.cer", "r")))
        {
          #ifdef SERIAL_DEBUG_ENABLED
          Serial.println("Failed to open pemCA.cer to read");
          #endif
          return;
        }
        char arraySettings[penCA.size()];

        MemoryManager memoryManager;

        int currentByte=0;
        int numberOfBytes = memoryManager.readLineFromFile(&penCA,&arraySettings[currentByte]);//-----BEGIN CERTIFICATE-----
        while(true){
          numberOfBytes = memoryManager.readLineFromFile(&penCA,&arraySettings[currentByte]);
          if(numberOfBytes==0||memcmp(&arraySettings[currentByte],"-----END CERTIFICATE-----",sizeof("-----END CERTIFICATE-----")-1)==0)break;
          currentByte+=numberOfBytes;
        }
        arraySettings[currentByte]=0;
        penCA.close();

        /*Serial.println(arraySettings);*/

        int decodedLen = base64_dec_len(arraySettings, currentByte);
        byte decoded[decodedLen];

        base64_decode((char*)decoded, arraySettings, currentByte);

        for(int i =0;i<decodedLen;i++)
        {
          char hexCar[2];
          sprintf(hexCar, "%02X", decoded[i]);
          if(i%16==15) Serial.println(hexCar);
          else Serial.print(hexCar);
        }

        Serial.println();
        Serial.println(decodedLen, DEC);

        /* sprintf(arraySettings,"{\"DOUTStateFormula\":\"");
        numberOfBytes+=memoryManager.readLineFromFile(&penCA,&arraySettings[numberOfBytes]);
        numberOfBytes+=sprintf(&arraySettings[numberOfBytes],"\"}");
        arraySettings[numberOfBytes]=0;*/
        char textFormatArray[22];
        if(getUTCTime(decoded,decodedLen,textFormatArray))Serial.print(textFormatArray);
        else Serial.print("error get UTCTime");

        apServerPtr->send(200, "application/json", arraySettings,currentByte);
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
