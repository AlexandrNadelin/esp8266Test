#ifndef __MQTT_CONNECTION_H
#define __MQTT_CONNECTION_H
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "MemoryManager.h"
#include <Ticker.h>
#include "TimeSpan.h"

/*const char pemCAMQTTConnector[] PROGMEM = R"EOF(
# DST Root CA X3    
-----BEGIN CERTIFICATE-----
MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/
MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT
DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow
PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD
Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB
AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O
rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq
OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b
xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw
7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD
aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV
HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG
SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69
ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr
AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz
R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5
JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo
Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ
-----END CERTIFICATE-----

)EOF";*/

/*void onsub(char *topic, byte *payload, unsigned int length)
{
    Serial.printf("Message arrived [%s]\n", topic);
}*/


class MQTTConnector
{
  public:
    MQTTConnector(){}
    ~MQTTConnector(){}
    
    void begin(MemoryManager* memoryManager)
    {
      this->memoryManager = memoryManager;
      this->mqttPort = atoi(memoryManager->networkProperty.mqttPort);
      this->mqttPublishPollingPeriod = atoi(memoryManager->networkProperty.mqttPublishPollingPeriod);

      WiFi.mode(WIFI_STA); 

      if(!(pemCA_cer = SPIFFS.open("/pemCA.cer", "r")))
      {
        #ifdef SERIAL_DEBUG_ENABLED
        Serial.println("Failed to open pemCA.cer to read ::MQTTConnector");
        #endif
        return;
      }
      else
      {
        Serial.println("Opened pemCA.cer to read ::MQTTConnector");

        //X509List Certificate(&pemCA_cer);//не работает, хотя File - наследник Stream

        size_t sizepemCA = pemCA_cer.size();
        char arayTmp[sizepemCA];
        pemCA_cer.read((uint8_t*)arayTmp,sizepemCA);
        Certificate.append(arayTmp);
        
        wifiClientSecure.setTrustAnchors(&Certificate);
        pemCA_cer.close();
      }
      //#ifdef MQTTS_USE_CLIENT_CERT
      //Serial.println("Using client certs");
      //wifi.setClientRSACert(&cert, &key);
      //#endif/
      mqtt.setClient(wifiClientSecure);
      mqtt.setServer(memoryManager->networkProperty.mqttServer, this->mqttPort);
      mqtt.setCallback/*(onsub);*/([](char *topic, byte *payload, unsigned int length){
        Serial.printf("Message arrived in topick: [%s], ", topic);
        char message[length+1];
        memcpy(message, payload, length);
        message[length]=0;
        Serial.printf("payload: [%s]\n", message);
      });
    }
    
    void loop()
    {  
      if(WiFi.status() != WL_CONNECTED)
      {    
        Serial.print("Connecting to ");
        Serial.println(memoryManager->networkProperty.stSSID);
        WiFi.begin(memoryManager->networkProperty.stSSID, memoryManager->networkProperty.stPassword);
        if (WiFi.waitForConnectResult() != WL_CONNECTED)return;
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());  

        // Set time via NTP, as required for x.509 validation
        configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

        Serial.print("Waiting for NTP time sync: ");
        time_t now = time(nullptr);
        while (now < 8 * 3600 * 2)
        {
          delay(500);
          Serial.print(".");
          now = time(nullptr);
        }
        Serial.println("");

        struct tm timeinfo;
        gmtime_r(&now, &timeinfo);
        Serial.print("Current time: ");
        Serial.print(asctime(&timeinfo));

        /*if(!(pemCA_cer = SPIFFS.open("/pemCA.cer", "r")))
        {
          #ifdef SERIAL_DEBUG_ENABLED
          Serial.println("Failed to open pemCA.cer to read");
          #endif
          return;
        }

        wifiClientSecure.setTrustAnchors(pemCA_cer);//&ca);//!!!
        pemCA_cer.close();

        //#ifdef MQTTS_USE_CLIENT_CERT
        //Serial.println("Using client certs");
        //wifi.setClientRSACert(&cert, &key);
        //#endif/

        mqtt.setServer(memoryManager->networkProperty.mqttServer, this->mqttPort);
        mqtt.setCallback(onsub);*/

        
      } 
      else 
      {
        if(!mqtt.connected())
        {
          Serial.print("Attempting MQTT connection...");

          if (mqtt.connect(memoryManager->networkProperty.mqttClientID, memoryManager->networkProperty.mqttUser, memoryManager->networkProperty.mqttPassword))
          {
            Serial.println("connected");
          
            mqtt.subscribe("base/relay/led1");
            mqtt.subscribe("base/state/adc");//"#");
            /*publishTicker.attach(mqttPublishPollingPeriod*0.001,[](){
              this->mqtt.publish("base/state/adc", "hello mqtts");
            });*/
            mqtt.publish("base/state/adc", "hello mqtts");
          }
          else
          {
            Serial.print("failed, rc=");
            Serial.print(mqtt.state());
          }
        }
        else 
        {
          mqtt.loop();//millis();

          unsigned long currentTime=millis();
          if(calculateTimeSpan(currentTime, previousPublishTime)>mqttPublishPollingPeriod)
          {
            mqtt.publish("base/state/adc", "hello mqtts");
            previousPublishTime=currentTime;
          }
        }
      }
    }
  private:
    //Ticker publishTicker;
    WiFiClientSecure wifiClientSecure;
    PubSubClient mqtt;//(wifiClientSecure);
    MemoryManager* memoryManager;
    File pemCA_cer;
    X509List Certificate;

    uint16_t mqttPort=8883;
    uint32_t mqttPublishPollingPeriod=2000;

    unsigned long previousPublishTime=0;
    /*void onsub(char *topic, byte *payload, unsigned int length)
    {
      Serial.printf("Message arrived [%s]\n", topic);
    }*/
};


#endif
