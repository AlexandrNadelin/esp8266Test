#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <EEPROM.h>

#import "pages.h"

#define D_IN_1  14
#define D_IN_2  12
#define D_IN_3  13
#define D_OUT_1 15

const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0
float sensorValue = 0;
uint8_t state_D_IN_1 =0;
uint8_t state_D_IN_2 =0;
uint8_t state_D_IN_3 =0;

const char* ap_ssid = "R_04_LOW_COST";
const char* ap_password = "AngryBird";

const char *st_ssid = "TP-LINK_453D2A"; 
const char *st_password = "36216703";

IPAddress apIP(192,168,5,1);
IPAddress apSubnet(255,255,255,0);
IPAddress apGateWay(192,168,5,1);
const uint16_t apPort = 80;

ESP8266WebServer apServer(apIP,apPort);

struct Memory
{
  char ap_ssid[24];
  char ap_password[24];
  char st_ssid[24];
  char st_password[24];
  unsigned char stIP[4];
  unsigned char stSubnet[4];
  unsigned char stGateWay[4];
  uint16_t stPort;
  uint16_t crc;
};

Memory memory;

void setup() {
  Serial.begin(115200);
  Serial.println();
  /*Serial.println();
  long int chipId = ESP.getChipId();
  Serial.print("ChipId: ");
  Serial.println(chipId);*/
  EEPROM.begin(sizeof(Memory));
  EEPROM.get(0,memory);//memory = EEPROM.read(0);
  uint8_t *ptr = (uint8_t*) &memory;
  uint16_t crc = 0;
  for(int i =0;i<sizeof(Memory)-2;i++){
    crc+=ptr[i];
  }  
  
  if(crc!=memory.crc){//first start
    strcpy(memory.ap_ssid,"R_04_LOW_COST");
    strcpy(memory.ap_password,"AngryBird");
    strcpy(memory.st_ssid,"TP-LINK_453D2A");
    strcpy(memory.st_password,"36216703");
    memory.stIP[0]=192;
    memory.stIP[1]=168;
    memory.stIP[2]=4;
    memory.stIP[3]=100;
    memory.stSubnet[0]=255;
    memory.stSubnet[1]=255;
    memory.stSubnet[2]=255;
    memory.stSubnet[3]=0;
    memory.stGateWay[0]=192;
    memory.stGateWay[1]=168;
    memory.stGateWay[2]=4;
    memory.stGateWay[3]=1;
    memory.stPort = 80;
    memory.crc=crc;
    EEPROM.put(0,memory);//EEPROM.write(0,memory);
    //EEPROM.commit();
    Serial.println("memory first start");
  }
  else Serial.println("memory second start");
  EEPROM.end();

  pinMode(D_IN_1, INPUT_PULLUP);
  pinMode(D_IN_2, INPUT_PULLUP);
  pinMode(D_IN_3, INPUT_PULLUP);
  pinMode(D_OUT_1, OUTPUT);
  digitalWrite(D_OUT_1, LOW);

  Serial.print("Setting soft-AP configuration ... ");
  Serial.println(WiFi.softAPConfig(apIP, apGateWay, apSubnet) ? "Ready" : "Failed!");

  Serial.print("Setting soft-AP ... ");
  Serial.println(WiFi.softAP(ap_ssid,ap_password) ? "Ready" : "Failed!");

  Serial.print("Soft-AP IP address = ");
  Serial.println(WiFi.softAPIP());

  apServer.on("/",[](){
    /*if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();*/ 
    apServer.send(200, "text/html", IndexAp_html, IndexAp_html_size); 
  }); 
  apServer.on("/style.css", [](){
    apServer.send(200, "text/css", style_css,style_css_size);
  }); 
  apServer.on("/R_04.settings",HTTP_GET, [](){//если не прописать get или post то сработает всегда первый запрос /R_04.settings, если проверить body скорее всего можно обойтись и без post
  
    char arraySettings[2000];
    int numberOfBytes = sprintf(arraySettings,"{\"ApSSID\":\"%s\",\"ApPASSWORD\":\"%s\",\"StSSID\":\"%s\",\"StPASSWORD\":\"%s\",\"StIPAddress\":\"%u.%u.%u.%u\",\"StSubnetMask\":\"%u.%u.%u.%u\",\"StGateway\":\"%u.%u.%u.%u\",\"StPort\":\"%u\"}"
    ,memory.ap_ssid
    ,memory.ap_password
    ,memory.st_ssid
    ,memory.st_password
    ,memory.stIP[0],memory.stIP[1],memory.stIP[2],memory.stIP[3]
    ,memory.stSubnet[0],memory.stSubnet[1],memory.stSubnet[2],memory.stSubnet[3]
    ,memory.stGateWay[0],memory.stGateWay[1],memory.stGateWay[2],memory.stGateWay[3]
    ,memory.stPort);
    
    arraySettings[numberOfBytes]=0;
    apServer.send(200, "application/json", arraySettings,numberOfBytes);
  });

  apServer.on("/R_04.settings",HTTP_POST, [](){
  String data = apServer.arg("plain"); 
  StaticJsonDocument<2000> doc;
  DeserializationError error = deserializeJson(doc, data);

  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }else Serial.println("/R_04.settings,HTTP_POST");

  String ApSSID = doc["ApSSID"];
  String ApPASSWORD = doc["ApPASSWORD"];
  String StSSID = doc["StSSID"];
  String StPASSWORD = doc["StPASSWORD"];
  String StIPAddress = doc["StIPAddress"];
  String StSubnetMask = doc["StSubnetMask"];
  String StGateway = doc["StGateway"];
  String StPort = doc["StPort"];

    /*Serial.println(ApSSID);
    Serial.println(ApPASSWORD);
    Serial.println(StSSID);
    Serial.println(StPASSWORD);
    Serial.println(StIPAddress);
    Serial.println(StSubnetMask);
    Serial.println(StGateway);
    Serial.println(StPort);*/
    
    apServer.send(200);
  });

  apServer.on("/R_04.reboot",HTTP_GET, [](){//

    apServer.send(200);
  });
  
  apServer.begin();

  Serial.println("HTTP server started");
  Serial.println();
}

void loop() {
  apServer.handleClient();

  sensorValue = analogRead(analogInPin)*3.3F/1024.0F;
  state_D_IN_1 = digitalRead(D_IN_1);
  state_D_IN_2 = digitalRead(D_IN_2);
  state_D_IN_3 = digitalRead(D_IN_3);
  if(0)digitalWrite(D_OUT_1, HIGH);
  // print the readings in the Serial Monitor
  /*Serial.print("sensor = ");
  Serial.println(sensorValue);
  Serial.println(digitalRead(D_IN_1));
  Serial.println(digitalRead(D_IN_2));
  Serial.println(digitalRead(D_IN_3));
  Serial.println();*/
  
  delay(1000);
}
