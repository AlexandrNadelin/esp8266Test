#ifndef __MQTT_CONNECTION_H
#define __MQTT_CONNECTION_H
#include <Arduino.h>
#include "Config.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include "MemoryManager.h"
#include "TimeSpan.h"
#include "PinsStateManager.h"


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
  
static PinsStateManager* pinsStateManagerPtr;


class MQTTConnector
{
  public:
    MQTTConnector(){}
    ~MQTTConnector(){}
    
    void begin(MemoryManager* memoryManager,PinsStateManager* pinsStateManager)
    {
      this->memoryManager = memoryManager;
      /*this->*/pinsStateManagerPtr=pinsStateManager;
      this->mqttPort = atoi(memoryManager->networkProperty.mqttPort);
      this->mqttPublishPollingPeriod = atoi(memoryManager->networkProperty.mqttPublishPollingPeriod);

      WiFi.mode(WIFI_STA); 

      if(this->mqttPort!=1883)
      {
        if(!(pemCA_cer = SPIFFS.open("/pemCA.cer", "r")))
        {
          #ifdef SERIAL_DEBUG_ENABLED
          Serial.println("Failed to open pemCA.cer to read ::MQTTConnector");
          #endif
          return;
        }
        else
        {
          #ifdef SERIAL_DEBUG_ENABLED
          Serial.println("Opened pemCA.cer to read ::MQTTConnector");
          #endif
          //X509List Certificate(&pemCA_cer);//не работает, хотя File - наследник Stream

          size_t sizepemCA = pemCA_cer.size();
          char arayTmp[sizepemCA];
          pemCA_cer.read((uint8_t*)arayTmp,sizepemCA);
          Certificate.append(arayTmp);
        
          wifiClientSecure.setTrustAnchors(&Certificate);
          //#ifdef MQTTS_USE_CLIENT_CERT
          //Serial.println("Using client certs");
          //wifi.setClientRSACert(&cert, &key);
          //#endif/
          pemCA_cer.close();
        }
      }
      else wifiClientSecure.setInsecure();//skip verification
      
      mqtt.setClient(wifiClientSecure);
      mqtt.setServer(memoryManager->networkProperty.mqttServer, this->mqttPort);
      mqtt.setCallback([](char *topic, byte *payload, unsigned int length){
        char message[length+1];
        memcpy(message, payload, length);
        message[length]=0;
        #ifdef SERIAL_DEBUG_ENABLED
        Serial.printf("Message arrived in topick: [%s], ", topic);
        Serial.printf("payload: [%s]\n", message);
        #endif

        if(memcmp(topic,"base/relay/dOut1",sizeof("base/relay/dOut1")-1)==0)
        {
          pinsStateManagerPtr->setDOUT1State(atoi(message));
        }
      });
    }
    
    void loop()
    {  
      if(WiFi.status() != WL_CONNECTED)
      { 
        #ifdef SERIAL_DEBUG_ENABLED   
          Serial.print("Connecting to ");
          Serial.println(memoryManager->networkProperty.stSSID);
        #endif
        WiFi.begin(memoryManager->networkProperty.stSSID, memoryManager->networkProperty.stPassword);
        if (WiFi.waitForConnectResult() != WL_CONNECTED)return;
        #ifdef SERIAL_DEBUG_ENABLED
          Serial.println("WiFi connected");
          Serial.println("IP address: ");
          Serial.println(WiFi.localIP());
        #endif  

        if(this->mqttPort!=1883)
        {
          // Set time via NTP, as required for x.509 validation
          configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
          #ifdef SERIAL_DEBUG_ENABLED
            Serial.print("Waiting for NTP time sync: ");
          #endif
          time_t now = time(nullptr);
          while (now < 8 * 3600 * 2)
          {
            delay(500);
            #ifdef SERIAL_DEBUG_ENABLED
              Serial.print(".");
            #endif
            now = time(nullptr);
          }
          #ifdef SERIAL_DEBUG_ENABLED
            Serial.println("");
          #endif

          struct tm timeinfo;
          gmtime_r(&now, &timeinfo);
          #ifdef SERIAL_DEBUG_ENABLED
            Serial.print("Current time: ");
            Serial.print(asctime(&timeinfo));
          #endif
        }
      } 
      else 
      {
        if(!mqtt.connected())
        {
          delay(500);
          Serial.print("Attempting MQTT connection...");

          if (mqtt.connect(memoryManager->networkProperty.mqttClientID, memoryManager->networkProperty.mqttUser, memoryManager->networkProperty.mqttPassword))
          {
            Serial.println("connected");
          
            mqtt.subscribe("base/relay/dOut1");
            mqtt.subscribe("base/state/data");//"#");
            //mqtt.publish("base/state/data", "hello mqtts");
          }
          else
          {
            #ifdef SERIAL_DEBUG_ENABLED
              Serial.print("failed, rc=");
              Serial.println(mqtt.state());
            #endif
          }
        }
        else 
        {
          mqtt.loop();

          unsigned long currentTime=millis();
          if(calculateTimeSpan(currentTime, previousPublishTime)>mqttPublishPollingPeriod)
          {
            char arrayTmp[256];
            sprintf(arrayTmp,"{\"A_IN\":\"%f\",\"D_IN_1\":\"%u\",\"D_IN_2\":\"%u\",\"D_OUT_1\":\"%u\"}",pinsStateManagerPtr->getAINState(),pinsStateManagerPtr->getDIN1State(),pinsStateManagerPtr->getDIN2State(),pinsStateManagerPtr->getDOUT1State());
            mqtt.publish("base/state/data", arrayTmp);//"hello mqtts");
            previousPublishTime=currentTime;
          }
        }
      }
    }
  private:
    //PinsStateManager* pinsStateManager;
    WiFiClientSecure wifiClientSecure;
    PubSubClient mqtt;//(wifiClientSecure);
    MemoryManager* memoryManager;
    File pemCA_cer;
    X509List Certificate;

    uint16_t mqttPort=8883;
    uint32_t mqttPublishPollingPeriod=2000;

    unsigned long previousPublishTime=0;
};


#endif
