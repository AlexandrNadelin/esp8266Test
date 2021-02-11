#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>

#import "pages.h"

MDNSResponder mdns;

// впишите сюда данные, соответствующие вашей сети: 
const char* ssid = "TP-LINK_453D2A";
const char* password = "36216703";

int sendCounter=0;

ESP8266WebServer server(80);

int gpio0_pin = 0;
int gpio2_pin = 2;

void setup(void){
  
  // подготавливаем GPIO-контакты:
  pinMode(gpio0_pin, OUTPUT);
  digitalWrite(gpio0_pin, LOW);
  pinMode(gpio2_pin, OUTPUT);
  digitalWrite(gpio2_pin, LOW);
  
  delay(1000);
  Serial.begin(115200);
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
  
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println("MDNS responder started");
               //  "Запущен MDNSresponder"
  }
  
  server.on("/", [](){
    server.send(200, "text/html", data_IndexStation_html,sizeof(data_IndexStation_html));
  });

  server.on("/style.css", [](){
    server.send(200, "text/css", data_style_css,sizeof(data_style_css));
  });  

  server.on("/R_04.parameters", [](){//?=
    char arraySettings[400];
    int numberOfBytes = sprintf(arraySettings,"{\"A_IN_1\":\"%.3f\",\"D_IN_1\":\"%u\",\"D_IN_2\":\"%u\",\"D_IN_3\":\"%u\",\"D_IN_4\":\"%u\",\"D_OUT_1\":\"%u\",\"Formula\":\"%s\"}",1.0F,2,3,4,sendCounter++,5,"1");
    server.send(200, "application/json", arraySettings,numberOfBytes);
    arraySettings[numberOfBytes]=0;
  });

  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  /*Serial.println("$Hallo world");
  delay(1000);
  digitalWrite(TXD, HIGH);
  delay(1000);
  digitalWrite(TXD, LOW);
  delay(1000);*/
}
