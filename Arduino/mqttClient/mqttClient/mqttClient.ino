#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Ticker.h>

Ticker dataSendTicker;
const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
float sensorValue = 0;
 //MosquittoBrokerLocal
const char* ssid = "TP-LINK_453D2A";
const char* password =  "36216703";
IPAddress mqttServer(192,168, 4, 216);//const char* mqttServer = "192.168.4.216";//"m11.cloudmqtt.com";
const int mqttPort = 1883;
const char* mqttUser = "MyMqttUser";
const char* mqttPassword = "MyMqttUserPassword";
 
WiFiClient espClient;
PubSubClient client(espClient);

void sendResultMeasuring(){
  if(!client.connected())
  {
    dataSendTicker.detach();
    return;
  }
  char adcStr[12];
  sensorValue = analogRead(analogInPin)*3.3F/1024.0F;
  sprintf(adcStr,"%f",sensorValue);
  boolean result = client.publish("esp/ADC", adcStr);
}
 
void setup(){ 
  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_STA);//(WIFI_AP_STA);
  //configure ap
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
 
        if(client.connect("ESP8266Client", mqttUser, mqttPassword ))Serial.println("connected"); 
        else
        { 
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000); 
        }
      }

      if(client.connected())
      {
        dataSendTicker.attach(1.5,sendResultMeasuring);
        client.subscribe("esp/test");
      }
    }
  }
  
  if(client.connected()) client.loop();
}
