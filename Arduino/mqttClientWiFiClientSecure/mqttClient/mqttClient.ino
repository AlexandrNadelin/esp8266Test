#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>
#include <WiFiClientSecure.h>
//#include <FS.h>

#import "rootCA.h"//Это ваш DST Root CA X3 в формате DER
#import "ca.h"    //Это мой клиентский сертификат переведенный из формата BASE64 в формат der
#import "key.h"   //Это мой RSA Private key переведенный из формата BASE64 в формат der

Ticker dataSendTicker;
const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
float sensorValue = 0;
 //MosquittoBrokerLocal
const char* ssid = "TP-LINK_453D2A";
const char* password =  "36216703";
const char* mqttServer = "dev.rightech.io";//"m11.cloudmqtt.com";//IPAddress mqttServer(192,168, 4, 216);//const char* mqttServer = "192.168.4.216";//"m11.cloudmqtt.com";
const int mqttPort = 8883;//1883;//
const char* mqttUser = "MyMqttUser";
const char* mqttPassword = "MyMqttUserPassword";
 
WiFiClientSecure wifiClientSecure;//чтобы была возможность шифрования TLS////WiFiClient wifiClientSecure;//
// Change #2: Set the SHA1 fingerprint for the connection -->
//const char fingerprint[] PROGMEM = "DA C9 02 4F 54 D8 F6 DF 94 93 5F B1 73 26 38 CA 6A D7 7C 13";

PubSubClient client(wifiClientSecure);

// You can use x.509 client certificates if you want
/*static const char rootCA[]=\
"-----BEGIN CERTIFICATE-----\n"\
"MIIDSjCCAjKgAwIBAgIQRK+wgNajJ7qJMDmGLvhAazANBgkqhkiG9w0BAQUFADA/\n"\
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n"\
"DkRTVCBSb290IENBIFgzMB4XDTAwMDkzMDIxMTIxOVoXDTIxMDkzMDE0MDExNVow\n"\
"PzEkMCIGA1UEChMbRGlnaXRhbCBTaWduYXR1cmUgVHJ1c3QgQ28uMRcwFQYDVQQD\n"\
"Ew5EU1QgUm9vdCBDQSBYMzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n"\
"AN+v6ZdQCINXtMxiZfaQguzH0yxrMMpb7NnDfcdAwRgUi+DoM3ZJKuM/IUmTrE4O\n"\
"rz5Iy2Xu/NMhD2XSKtkyj4zl93ewEnu1lcCJo6m67XMuegwGMoOifooUMM0RoOEq\n"\
"OLl5CjH9UL2AZd+3UWODyOKIYepLYYHsUmu5ouJLGiifSKOeDNoJjj4XLh7dIN9b\n"\
"xiqKqy69cK3FCxolkHRyxXtqqzTWMIn/5WgTe1QLyNau7Fqckh49ZLOMxt+/yUFw\n"\
"7BZy1SbsOFU5Q9D8/RhcQPGX69Wam40dutolucbY38EVAjqr2m7xPi71XAicPNaD\n"\
"aeQQmxkqtilX4+U9m5/wAl0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNV\n"\
"HQ8BAf8EBAMCAQYwHQYDVR0OBBYEFMSnsaR7LHH62+FLkHX/xBVghYkQMA0GCSqG\n"\
"SIb3DQEBBQUAA4IBAQCjGiybFwBcqR7uKGY3Or+Dxz9LwwmglSBd49lZRNI+DT69\n"\
"ikugdB/OEIKcdBodfpga3csTS7MgROSR6cz8faXbauX+5v3gTt23ADq1cEmv8uXr\n"\
"AvHRAosZy5Q6XkjEGB5YGV8eAlrwDPGxrancWYaLbumR9YbK+rlmM6pZW87ipxZz\n"\
"R8srzJmwN0jP41ZL9c8PDHIyh8bwRLtTcm1D9SZImlJnt1ir/md2cXjbDaJWFBM5\n"\
"JDGFoqgCWjBH4d1QB7wCCZAA62RjYJsWvIjJEubSfZGL+T0yjWW06XyxV3bqxbYo\n"\
"Ob8VZRzI9neWagqNdwvYkQsEjgfbKbYK7p2CNTUQ\n"\
"-----END CERTIFICATE-----\n";

static const char certificate[]=\
"-----BEGIN CERTIFICATE-----\n"\
"MIIEiDCCAnCgAwIBAgIULoOoEWP7U6HWXZzC6to+zejxH5YwDQYJKoZIhvcNAQEL\n"\
"BQAwFzEVMBMGA1UEAxMMUklDLUludGVyLUNBMB4XDTIxMDIxNzA4NDUxN1oXDTI0\n"\
"MDIwMjA4NDU0N1owNzE1MDMGA1UEAxMsNjAyN2YwZDRlMzFlMzUwMDEwOGExMTU3\n"\
"Lm9iamVjdHMucmlnaHRlY2guaW8wggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK\n"\
"AoIBAQDC2aM/OcROU56N0NfpZXOA5A851jdJqQGbK8izPS4S0kNjYUS73aoVZiJh\n"\
"YQCf38Rucj+py0h0bGWJKjEFLzZsGzcLLwISLJOx+4izylLaEm+XC4WsEi5TMZqC\n"\
"tObpKHRIQydmsGTr04s9WZXZ3DgU5Z4ZupOynWLLyOOzr0I4sbEA7oWWN1uwZ/HP\n"\
"K/tKOKbab6Px7l0VPS5ZkmWgRo8yckB4ErexYDP0D+po9H/j4QVm/nf98Ip83KSQ\n"\
"yYO5yIjsn0hALSA6ZkuCInYWnXnalE4qmn8XMrEIRuKORR8n+9OflUhjCOxtSOLo\n"\
"xYiQshuRJiZT/tjksjabwOMYglq1AgMBAAGjgaswgagwDgYDVR0PAQH/BAQDAgOo\n"\
"MB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAdBgNVHQ4EFgQUZg41hRMv\n"\
"hMK71PIG9wpLkDyRNi4wHwYDVR0jBBgwFoAUmP7inKw4RUikvtMwJCI9+NxnCaMw\n"\
"NwYDVR0RBDAwLoIsNjAyN2YwZDRlMzFlMzUwMDEwOGExMTU3Lm9iamVjdHMucmln\n"\
"aHRlY2guaW8wDQYJKoZIhvcNAQELBQADggIBAH3nR8erjLbIbe1UigHQ1Lvd2tkK\n"\
"+2Qq9VASPXzuglz2rCimBDMR1JqtLUh21Wt3Ke7lKEQ7psI9Dq7l5Wsoj7JcK5Ak\n"\
"SVbwJ+jeOaztEWvPXEMbG/AAZ8gHh3HCyKqWrWPtsnayD4TMLiijYmZIEPcQR71L\n"\
"gSPaht32w3n9NM1mmd2BZdRiPb1DExkHfHB81gmQwZP/b3/jtPXGl4ZSeQFt6W5B\n"\
"7jgNFe0lUJB9b5wGn2T/bCG33EV92ngi5LOG6LThcXzsYBAjFvmbWJUkYZFSGQgO\n"\
"qqmIKgCfV9xDS2g0voJNY216nyR70QS+KVUK9R/qwCu2GfTq4DHefmEifYmOGyQO\n"\
"Y8/Fn584Fbnrhp92woDfapPV1ABwSoaAPDQdde4WGNr/mYjr6QGgdt8LsGiLS1Rs\n"\
"oTwIIfcuq5R8xuiZ1Q5HImfmU159Ny539ie5pn1rfOzRBIXUC+ke7koqG3i/BjgK\n"\
"2JaiCRpYxpvrbolOG9QgxUaGPEKTIu4qFYhsphbj/gBX0ldKi3Ddju4nEdymMXN2\n"\
"ZCwrDQ9YemUWM0ciUJ4LlRUF1Q8dYS4eYx/VcP1Gs7oHeJemwBz8JkWNkCiiG/Fy\n"\
"ibVHaCqaPn1QglzlmYudXhjhIr8xayhoxlspkxMA/W7gvViBAqAa/LfGzIA1jYWw\n"\
"pXvOFH3vcBEpOYIr\n"\
"-----END CERTIFICATE-----\n";

static const char rsaPrivateKey[]=\
"-----BEGIN RSA PRIVATE KEY-----\n"\
"MIIEpAIBAAKCAQEAwtmjPznETlOejdDX6WVzgOQPOdY3SakBmyvIsz0uEtJDY2FE\n"\
"u92qFWYiYWEAn9/EbnI/qctIdGxliSoxBS82bBs3Cy8CEiyTsfuIs8pS2hJvlwuF\n"\
"rBIuUzGagrTm6Sh0SEMnZrBk69OLPVmV2dw4FOWeGbqTsp1iy8jjs69COLGxAO6F\n"\
"ljdbsGfxzyv7Sjim2m+j8e5dFT0uWZJloEaPMnJAeBK3sWAz9A/qaPR/4+EFZv53\n"\
"/fCKfNykkMmDuciI7J9IQC0gOmZLgiJ2Fp152pROKpp/FzKxCEbijkUfJ/vTn5VI\n"\
"YwjsbUji6MWIkLIbkSYmU/7Y5LI2m8DjGIJatQIDAQABAoIBAQCdbii20d6pBmjJ\n"\
"3l5ExmJfVmRy74B2NGYG0IZ1Q04xiM9HwXQ+ceeVKJNIqI5PngVg01BayNM+tcIH\n"\
"5B39OVs4V3jOwWlj875VhIy77xGn/lNI79dN4tapAx/60VzsL3JcXF2+W3D/FUmg\n"\
"BPzNLF/nimqGETh6jS0gx6wqJsVTddsAq7QEdfoghYQIO4T6K0mLNbZ1lZiSWc9k\n"\
"lK6mRDaDAP8zQbLK1YpiwV7cCUFLNJlGh90uBwW1gzJ7FBQ+FcdMQcUlkUtkmNta\n"\
"7CcCELpNPTe7hmmy7u3b8NGAbh1NfKza7y++54GYn4P1412T315JD1tg56xaA8js\n"\
"wNLKbLD1AoGBAMeHZYxCEgQFEtnITiG0KJubvoWqb76/oigcDC5FvjgLlMJaCLVp\n"\
"a2Ye6LZHIy7sAhh8oLOIyTd2Bq4TOG1ieDz3/Q2hAoPbsfCshFRSeh+Tl/ClreIQ\n"\
"mOwMmpaho0SC9TDtnz/oPpDgxeQk95s6W9nXennsGfWRhhjIi7lfHaYvAoGBAPn/\n"\
"P6XvVCnM6mjb02f0MTOPBzSvvT7zoR4GY7SP1xajGBjueeljwj836+sHtIFE4y61\n"\
"sSb0ZCQJO/bqJAt6D2xkIpZUKNABLoHtthBU0kTQ/4+n6DtqzP5he5rGw6PUyaFR\n"\
"tSiMKQDYYGo1Vwat6//2UXsU1Zv2RhG54ouzwjhbAoGAKGm6HbB8KahFayrw+E6f\n"\
"MvVWn4gn3gC+fVVA+lZR8tQVq0/FF8lkYWi9Rb4e2DAhNIh5nCOGbd9P9w4Zjb42\n"\
"EFNCQ/Jqqf5jihXHL7UM2zxvt+Q21eT6tETRQtU2VngX5XMboDwQABQmwNDj/eLS\n"\
"G8tTGULC90QkXvyvoDotrS0CgYEAx92cWiIzzFNqWLjelC5zI4BGx4V0rB6FAgcj\n"\
"sViKIivz+QK3brL1uisQLs/zLKIRaueRoM4kPXcwfKAGfzJvoD2SNVNFab54GyIb\n"\
"u4TEsoXQu14G2XHyP7FZ64zbHmDhblpvleIwad+TfXX1hH6JvhKEY/1tJYpHzKYr\n"\
"C3uEXB8CgYAd4C7j+D9naeCxyQo3hd87krPwA8+afg/vexnTQ8DbbWWZEti5paKj\n"\
"X3PdTkgp6yK806nnEFxObQ6UY9ANHTx0yxj0+80mPmNwlL+oy2HBOUNeefZgfzRy\n"\
"14SRi4ucYrOM/to8hNPKjy/c1vE+B3cvnYSVOFYdYTrPqLVwWfO6eQ==\n"\
"-----END RSA PRIVATE KEY-----\n";*/

void sendResultMeasuring(){
  if(!client.connected())
  {
    dataSendTicker.detach();
    return;
  }
  char adcStr[12];
  sensorValue = analogRead(analogInPin)*3.3F/1024.0F;
  sprintf(adcStr,"%f",sensorValue);
  boolean result = client.publish("base/state/adc", adcStr);
}
 
void setup(){ 
  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_STA);//(WIFI_AP_STA);
  //configure ap

  /*if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  File ca = SPIFFS.open("/ca.crt", "r"); //replace ca.crt eith your uploaded file name
  if (!ca) Serial.println("Failed to open ca file");
  else Serial.println("Success to open ca file");

  if(wifiClientSecure.loadCertificate(ca))Serial.println("loaded ca");
  else Serial.println("not loaded ca");

  File key = SPIFFS.open("/key.crt", "r"); //replace ca.crt eith your uploaded file name
  if (!key) Serial.println("Failed to open key file");
  else Serial.println("Success to open key file");

  if(wifiClientSecure.loadPrivateKey(key))Serial.println("loaded key");
  else Serial.println("not loaded key");*/

  /*if (!wifiClientSecure.setCACert(root_ca, sizeof(root_ca)))
  {
    Serial.println("Failed to load CA certificate");
  }*/

  //Serial.println(sizeof(certificate));
  /*wifiClientSecure.setCACert(rootCA);
  wifiClientSecure.setCertificate(certificate);
  wifiClientSecure.setPrivateKey(rsaPrivateKey);*/
  
  if(!wifiClientSecure.setCACert_P(derRootCA_cer,derRootCA_cerLength))Serial.println("Failed to load rootCA");//setCACert
  if(!wifiClientSecure.setCertificate(derCa_cer,derCa_cerLength))Serial.println("Failed to load Certificate");
  if(!wifiClientSecure.setPrivateKey(derKey_cer,derKey_cerLength))Serial.println("Failed to load key");
  //if (!wifiClientSecure.setFingerprint(fingerprint))Serial.println("Failed to load fingerprint");
  //wifiClientSecure.setInsecure();//skip verification
  //wifiClientSecure.setFingerprint(fingerprint);
}
 
void callback(char* topic, byte* payload, unsigned int length)
{ 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
  }
 
  Serial.println();
  Serial.println("-----------------------"); 
}
 
void loop() {
  if(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    //WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)return;
    
    //delay(1000);
    //if(WiFi.status() != WL_CONNECTED)Serial.println("Connecting to WiFi..");
    //else
    if(WiFi.status() == WL_CONNECTED)
    {
      Serial.println("Connected to the WiFi network");
 
      client.setServer(mqttServer, mqttPort);
      client.setCallback(callback);
 
      while (!client.connected()&&WiFi.status() == WL_CONNECTED)
      {
        Serial.println("Connecting to MQTT...");
 
        if(client.connect("mqtt-dgonny1986-ivii9d", mqttUser, mqttPassword ))Serial.println("connected"); //boolean connect (clientID, [username, password], [willTopic, willQoS, willRetain, willMessage], [cleanSession])
        else
        { 
          Serial.print("failed with state ");
          Serial.println(client.state());
          delay(10000); 
        }
      }

      if(client.connected())
      {
        dataSendTicker.attach(1.5,sendResultMeasuring);
        client.subscribe("base/state/adc");
      }
    }
  }
  
  if(client.connected()) client.loop();
}
