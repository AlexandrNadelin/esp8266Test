#ifndef testLib_h
#define testLib_h
#include <Arduino.h>
#include "Config.h"

struct NetworkProperty
{
  char apSSID[28];
  char apPassword[28];
  char apIP[16];
  char apSubnet[16];
  char apGateWay[16];
  
  char stSSID[28];
  char stPassword[28];
  
  char mqttServer[30]; 
  char mqttPort[6]; 
  char mqttClientID[28];  
  char mqttUser[28];  
  char mqttPassword[28];  
  char mqttPublishPollingPeriod[12];
  
  char doutFormula[28];
};

const char apSSID[] PROGMEM =       "R_04_LOW_COST";//"\"apSSID\": \"R_04_LOW_COST\",";
const char apPassword[] PROGMEM =   "AngryBird";//"\"apPassword\": \"AngryBird\",";
const char apIP[] PROGMEM =         "192.168.5.1";//"\"apIP\": \"192.168.5.1\",";
const char apSubnet[] PROGMEM =     "255.255.255.0";//"\"apSubnet\": \"255.255.255.0\",";
const char apGateWay[] PROGMEM =    "192.168.5.1";//"\"apGateWay\": \"192.168.5.1\",";

const char stSSID[] PROGMEM =       "TP-LINK_453D2A";//"\"stSSID\": \"TP-LINK_453D2A\",";
const char stPassword[] PROGMEM =   "36216703";//"\"stPassword\": \"36216703\",";

const char mqttServer[] PROGMEM =   "dev.rightech.io";//"\"mqttServer\": \"dev.rightech.io\",";
const char mqttPort[] PROGMEM =     "8883";//"1883"
const char mqttClientID[] PROGMEM = "mqtt-dgonny1986-ivii9d";//"\"mqttClientID\": \"mqtt-dgonny1986-ivii9d\",";
const char mqttUser[] PROGMEM =     "MyMqttUser";//"\"mqttUser\": \"MyMqttUser\",";
const char mqttPassword[] PROGMEM = "MyMqttUserPassword";//"\"mqttPassword\": \"MyMqttUserPassword\",";
const char mqttPublishPollingPeriod[] PROGMEM = "2000";//"\"mqttPassword\": \"MyMqttUserPassword\",";

/*const char doutFormula[] PROGMEM =  "0";*///"\"doutFormula\": \"0\",";

static const int arrayPropertiesLenght = 13;

const char* const arrayProperties[] PROGMEM = {
  apSSID, apPassword, apIP,apSubnet,apGateWay,stSSID,stPassword,mqttServer,mqttPort,mqttClientID,mqttUser,mqttPassword,mqttPublishPollingPeriod/*,doutFormula*/
};

const char pemCA[] PROGMEM = R"EOF(
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

)EOF";
/*

# ISRG Root X1 (Self-signed)
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----

# ISRG Root X1 (Cross-signed by DST Root CA X3)
-----BEGIN CERTIFICATE----- 
MIIFYDCCBEigAwIBAgIQQAF3ITfU6UK47naqPGQKtzANBgkqhkiG9w0BAQsFADA/
MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT
DkRTVCBSb290IENBIFgzMB4XDTIxMDEyMDE5MTQwM1oXDTI0MDkzMDE4MTQwM1ow
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwggIiMA0GCSqGSIb3DQEB
AQUAA4ICDwAwggIKAoICAQCt6CRz9BQ385ueK1coHIe+3LffOJCMbjzmV6B493XC
ov71am72AE8o295ohmxEk7axY/0UEmu/H9LqMZshftEzPLpI9d1537O4/xLxIZpL
wYqGcWlKZmZsj348cL+tKSIG8+TA5oCu4kuPt5l+lAOf00eXfJlII1PoOK5PCm+D
LtFJV4yAdLbaL9A4jXsDcCEbdfIwPPqPrt3aY6vrFk/CjhFLfs8L6P+1dy70sntK
4EwSJQxwjQMpoOFTJOwT2e4ZvxCzSow/iaNhUd6shweU9GNx7C7ib1uYgeGJXDR5
bHbvO5BieebbpJovJsXQEOEO3tkQjhb7t/eo98flAgeYjzYIlefiN5YNNnWe+w5y
sR2bvAP5SQXYgd0FtCrWQemsAXaVCg/Y39W9Eh81LygXbNKYwagJZHduRze6zqxZ
Xmidf3LWicUGQSk+WT7dJvUkyRGnWqNMQB9GoZm1pzpRboY7nn1ypxIFeFntPlF4
FQsDj43QLwWyPntKHEtzBRL8xurgUBN8Q5N0s8p0544fAQjQMNRbcTa0B7rBMDBc
SLeCO5imfWCKoqMpgsy6vYMEG6KDA0Gh1gXxG8K28Kh8hjtGqEgqiNx2mna/H2ql
PRmP6zjzZN7IKw0KKP/32+IVQtQi0Cdd4Xn+GOdwiK1O5tmLOsbdJ1Fu/7xk9TND
TwIDAQABo4IBRjCCAUIwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAQYw
SwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5pZGVudHJ1
c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTEp7Gkeyxx
+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEEAYLfEwEB
ATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2VuY3J5cHQu
b3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0LmNvbS9E
U1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFHm0WeZ7tuXkAXOACIjIGlj26Ztu
MA0GCSqGSIb3DQEBCwUAA4IBAQAKcwBslm7/DlLQrt2M51oGrS+o44+/yQoDFVDC
5WxCu2+b9LRPwkSICHXM6webFGJueN7sJ7o5XPWioW5WlHAQU7G75K/QosMrAdSW
9MUgNTP52GE24HGNtLi1qoJFlcDyqSMo59ahy2cI2qBDLKobkx/J3vWraV0T9VuG
WCLKTVXkcGdtwlfFRjlBz4pYg1htmf5X6DYO8A4jqv2Il9DjXA6USbW1FzXSLr9O
he8Y4IWS6wY7bCkjCWDcRQJMEhg76fsO3txE+FiYruq9RUWhiF1myv4Q6W+CyBFC
Dfvp7OOGAN6dEOM4+qR9sdjoSYKEBpsr6GtPAQw4dy753ec5
-----END CERTIFICATE-----*/

class MemoryManager {    
  public:
    MemoryManager(){}//constructor
    ~MemoryManager(){}//destructor

    File* getNetworkParametersFilePtr()
    {
      return &networkParameters;      
    }

    File* getPemCA_cerFilePtr()
    {
      return &pemCA_cer;      
    }
  
    int readLineFromFile(File* file,char* destString)
    {
      byte byteTmp=0;
      int currentByte = 0; 
      while (file->available())
      {//int l = file.readBytesUntil('\n', buffer, sizeof(buffer));
        byteTmp=file->read(); 
        if((char)byteTmp=='\n')break;
        else if((char)byteTmp!='\r') destString[currentByte++] = byteTmp;//'\r' skip
      } 
      destString[currentByte] = 0;
      return currentByte; 
    }

    bool writeDefaultNetworkParametersToFile()
    {
      if(!(networkParameters = SPIFFS.open("/NetworkParameters.property", "w")))
      {
        #ifdef SERIAL_DEBUG_ENABLED
        Serial.println("Failed to open networkParameters.property to write");
        #endif
        return false;
      }
      for(int j = 0; j < arrayPropertiesLenght;j++)
      {
        for(int i = 0; i < strlen_P(arrayProperties[j]); i++)
        {
          networkParameters.print((char)pgm_read_byte(&arrayProperties[j][i]));
        }
        networkParameters.println();
      }
 
      networkParameters.close(); 
      return true;
    }
    
	  void begin()
    {     
      bool resultMountinFileSystem = SPIFFS.begin();
 
      if (resultMountinFileSystem)
      {
        #ifdef SERIAL_DEBUG_ENABLED
        Serial.println("File system mounted with success");
        #endif
      }
      else 
      {
        #ifdef SERIAL_DEBUG_ENABLED
        Serial.println("Error mounting the file system");
        #endif
        return;
      } 
  
      while(!(networkParameters = SPIFFS.open("/NetworkParameters.property", "r")))
      {
        #ifdef SERIAL_DEBUG_ENABLED
        Serial.println("Failed to open networkParameters.property to read");
        #endif

        if(!writeDefaultNetworkParametersToFile())return;
      }

      #ifdef SERIAL_DEBUG_ENABLED
      Serial.println("Success to open networkParameters.property");
      #endif
      //size_t sent = server.streamFile(file, contentType);    // Send it to the client
      /*while (networkParameters.available()) {//int l = file.readBytesUntil('\n', buffer, sizeof(buffer));
      //Serial.print(networkParameters.read());  //����� �������� �� ������  0x00
      Serial.write(networkParameters.read());    //����� �������� ��� �����
      }*/

      //Если не получится получить все данные - надо записать дефолтные значения
  
      if(readLineFromFile(&networkParameters,networkProperty.apSSID)==0
       ||readLineFromFile(&networkParameters,networkProperty.apPassword)==0
       ||readLineFromFile(&networkParameters,networkProperty.apIP)==0
       ||readLineFromFile(&networkParameters,networkProperty.apSubnet)==0
       ||readLineFromFile(&networkParameters,networkProperty.apGateWay)==0
  
       ||readLineFromFile(&networkParameters,networkProperty.stSSID)==0
       ||readLineFromFile(&networkParameters,networkProperty.stPassword)==0

       ||readLineFromFile(&networkParameters,networkProperty.mqttServer)==0
       ||readLineFromFile(&networkParameters,networkProperty.mqttPort)==0
       ||readLineFromFile(&networkParameters,networkProperty.mqttClientID)==0
       ||readLineFromFile(&networkParameters,networkProperty.mqttUser)==0
       ||readLineFromFile(&networkParameters,networkProperty.mqttPassword)==0
       ||readLineFromFile(&networkParameters,networkProperty.mqttPublishPollingPeriod)==0)
       {
         #ifdef SERIAL_DEBUG_ENABLED
         Serial.println("networkParameters.property reading error, write default settings");
         #endif
         networkParameters.close();
         writeDefaultNetworkParametersToFile();
       }
       else networkParameters.close();
      /*readLineFromFile(&networkParameters,networkProperty.doutFormula);*/
  
       

      Serial.println(networkProperty.apSSID);
      Serial.println(networkProperty.apPassword);
      Serial.println(networkProperty.apIP);
      Serial.println(networkProperty.apSubnet);
      Serial.println(networkProperty.apGateWay);
  
      Serial.println(networkProperty.stSSID);
      Serial.println(networkProperty.stPassword);
  
      Serial.println(networkProperty.mqttServer);
      Serial.println(networkProperty.mqttPort);
      Serial.println(networkProperty.mqttClientID);
      Serial.println(networkProperty.mqttUser);//mqttPublishPollingPeriod
      Serial.println(networkProperty.mqttPassword);
      Serial.println(networkProperty.mqttPublishPollingPeriod);
  
      /*Serial.println(networkProperty.doutFormula);*/

      while(!(pemCA_cer = SPIFFS.open("/pemCA.cer", "r")))
      {
        #ifdef SERIAL_DEBUG_ENABLED
        Serial.println("Failed to open pemCA.cer to read");
        #endif

        if (!(pemCA_cer = SPIFFS.open("/pemCA.cer", "w")))
        {
          #ifdef SERIAL_DEBUG_ENABLED
          Serial.println("Failed to open pemCA.cer to write");
          #endif
          return;
        }
        //int bytesWritten = networkParameters.print("TEST SPIFFS");
        //int bytesWriten = networkParameters.write(&RTC_cache_data[0], RTC_cache.writePos);
        for(int i = 0; i < strlen_P(pemCA); i++)
        {
          pemCA_cer.print((char)pgm_read_byte(&pemCA[i])); 
        }
 
        pemCA_cer.close();    
      }

      #ifdef SERIAL_DEBUG_ENABLED
      Serial.println("Success to open pemCA.cer");
      #endif
      //Serial.println("file contents: ");
      //size_t sent = server.streamFile(file, contentType);    // Send it to the client
      /*while (pemCA_cer.available()) {//int l = file.readBytesUntil('\n', buffer,             sizeof(buffer));   
      Serial.write(pemCA_cer.read());
      }*/
      networkParameters.close(); 
    }
    
    NetworkProperty networkProperty;
  private:
    File networkParameters;
    File pemCA_cer;
};


#endif
