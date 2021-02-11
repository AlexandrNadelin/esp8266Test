/*
    This sketch establishes a TCP connection to a "quote of the day" service.
    It sends a "hello" message, and then prints received data.
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>


#import "indexUPUP.h"
#import "gif1UPUP.h"
#import "homeUPUP.h";
#import "pabb_gif.h"
#import "network.h"
#import "rsParam.h"
#import "upload.h"

const char* ssid     = "ZyXEL";//"Rostelecom_9C88";
const char* password = "12345678";//"p64aKuVy";

const char* host         = "192.168.4.218";
const uint16_t port      = 9999;
const char* www_username = "UPUP";
const char* www_password = "6728";

IPAddress ipLocal(192,168,4,226);
IPAddress ipMask(255,255,255,0);
IPAddress ipGateWay(192,168,4,1);
IPAddress ipHost(192,168,4,218);

 WiFiClient client;
 ESP8266WebServer server(80);

const uint16_t portServer = 10002;  //Port number
WiFiServer TCPserver(portServer);//(ipHost, portServer);

unsigned char datIsReady;
int countByteDat;
uint8_t buf[512];
uint16_t crc;
unsigned long timeout;

 void handleRoot() {
  if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();
 
  server.send(200, "text/html", &MAIN_page[0], sizeof(MAIN_page));  
}
void Llblue_gif(){
  server.send(200, "image/gif", &gif[0], sizeof(gif));
}
void Home_gif(){
  server.send(200, "image/gif", &gif1[0], sizeof(gif1));
}
void LanSetting(){
  server.send(200, "text/html", &net[0], sizeof(net));
}
void PabbGif(){
  server.send(200, "image/gif", &gif2[0], sizeof(gif2));
}
void IpSet(){
  char dat[18];
  snprintf(dat, 18, "%d.%d.%d.%d", 192,168,4,226);
  server.send(200, "text/html", dat);
}
void hahdleForm(){
  String ip = server.arg("ip_set");
  String mask = server.arg("msk");
  String gateway = server.arg("gw");
  String mac = server.arg("mac");
  Serial.print("$Network_setting");
  Serial.println(ip);
  Serial.println(mask);
  Serial.println(gateway);
  Serial.println(mac);
  LanSetting();
}
void sendFormDat(){
  String s = "192.168.4.226,255.255.255.0,192.168.4.1,1B-1C-02-14-23-56,1.3,";
  server.send(200, "text/html", s);
}
void handleRS(){
  server.send(200, "text/html", &rs[0], sizeof(rs));
}
void sendFormDatRs(){
  String s = "1,1,1,1,1,1,1,";
  server.send(200, "text/html", s);
}
void handleDatFormRs(){
  String baud = server.arg("Baud");
  String dataBits = server.arg("Data");
  String stopBits = server.arg("Stop");
  String parity = server.arg("Parity");
  String hand = server.arg("Hand");
  String crSelect = server.arg("Simvol");
  String empty = server.arg("NullSim");
  Serial.println("$");
  Serial.println("Baud:"); Serial.print(baud);
  Serial.println();
  Serial.println("dataBits:"); Serial.print(dataBits);
  Serial.println();
  Serial.println("stopBits:"); Serial.print(stopBits);
  Serial.println();
  Serial.println("parity:"); Serial.print(parity);
  Serial.println();
  Serial.println("hand:"); Serial.print(hand);
  Serial.println();
  Serial.println("crSelect:"); Serial.print(crSelect);
  Serial.println();
  Serial.println("empty:"); Serial.print(empty);
  handleRS();
}
void handleUpload(){
  server.send(200, "text/html", &upload[0], sizeof(upload));
}
/*******************************************************************************************************/
//File UploadFile;
String webpage = "";

String file_size(int bytes){
  String fsize = "";
  if (bytes < 1024)                 fsize = String(bytes)+" B";
  else if(bytes < (1024*1024))      fsize = String(bytes/1024.0,3)+" KB";
  else if(bytes < (1024*1024*1024)) fsize = String(bytes/1024.0/1024.0,3)+" MB";
  else                              fsize = String(bytes/1024.0/1024.0/1024.0,3)+" GB";
  return fsize;
}

void append_page_header() {
 webpage += F("<!DOCTYPE html><html><head>");
 webpage += F("<meta http-equiv='Content-Type' content='text/html; charset=utf-8'/>");
 webpage += F("<title>File upload</title></head>");
 webpage += F("<body><div align=center style='width: 833; height: 470'>");
 webpage += F("<table style='border: 1px solid #000080' height=384 cellSpacing=0 cellPadding=0 width=815>");
 webpage += F("<tbody><tr style='background-color: green'><td style='border-bottom: 1px solid #000080'></td></tr>");
 webpage += F("<tr><td colSpan=5 height=380 width=815 vAlign=top style='background-repeat: repeat-y;'>");
 webpage += F("<div align=center><font face='verdana'><table width=90% border=0 align=center><tbody><tr><td width=95%>");
}

void datCrcAndSend(uint8_t* data, int size){
  crc = calculationCRC(data, size);
  for(int i = 0; i < size; i++) Serial.write(data[i]);
  uint8_t ch = crc & 0xFF;
  Serial.write(ch);
  ch = (crc >> 8) & 0xFF;
  Serial.write(ch);
}

uint8_t ErrorUpdate;
uint8_t StartUpdate;

void handleFileUpload(){ // upload a new file to the Filing system
  HTTPUpload& uploadfile = server.upload();  
  if(uploadfile.status == UPLOAD_FILE_START)
  {
    String filename = uploadfile.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
    //Serial.print("$Upload File Name: "); Serial.println(filename);
    //отправляем команду на обновление 0xC8 crc
    buf[0] = 200;
    buf[1] = 0;
    datCrcAndSend(&buf[0], 1);
    timeout = millis();
    //ждём ответа 0x80 0xC8 crc
    countByteDat = 0;
    ErrorUpdate = 0;
    StartUpdate = 1;
    while(true){
      if (millis() - timeout > 5000) {ErrorUpdate = 1; break;}
      if(Serial.available()>0){
        while(Serial.available()>0)    //Checks is there any data in buffer 
         {
          if(countByteDat < sizeof(buf)) {
            buf[countByteDat++] = Serial.read();
           }
          else buf[0] = Serial.read(); 
          delay(1);
         }
       }
       if(countByteDat > 0){
         if(buf[0] == 0x80 && buf[1] == 0xC8){
          ErrorUpdate = 0;
         }
         else{
          ErrorUpdate = 1;
         }
         break;
       }
    }
    countByteDat = 0;
    //filename = String();
    //Serial.print("$ Name: "); Serial.println(filename);
  }
  else if (uploadfile.status == UPLOAD_FILE_WRITE)
  {
    if(ErrorUpdate == 1) return;
    //Serial.print("UPLOAD_FILE_WRITE:"); Serial.println(uploadfile.currentSize);
    int sizeFile = uploadfile.currentSize;
    //uploadfile.buf[2048]
    uint8_t countPacket = sizeFile / 256; //число пакетов по 256 байт
    if(countPacket * 256 < sizeFile) countPacket++;
    for(int j = 0; j < countPacket; j++){
      if(sizeFile >= 256){
        datCrcAndSend(&uploadfile.buf[j * 256], 256);
        sizeFile = sizeFile - 256;
      }
      else{ 
        datCrcAndSend(&uploadfile.buf[j * 256], sizeFile);
        sizeFile = 0;
      }
        timeout = millis();
        countByteDat = 0;
        while(true){
          if (millis() - timeout > 5000) {ErrorUpdate = 1; break;}
          if(Serial.available()>0){
           while(Serial.available()>0)    //Checks is there any data in buffer 
           {
              if(countByteDat < sizeof(buf)) {
                 buf[countByteDat++] = Serial.read();
               }
             delay(1);
           }
          }
          if(countByteDat > 0){
             if(buf[0] == 0x80 && buf[1] == 0xC8){
                ErrorUpdate = 0;
              }
             else{
                ErrorUpdate = 1;
              }
            break;
          }
       }
        //Serial.print("UPLOAD_256_WRITE:"); Serial.println("*****************");
    }
  } 
  else if (uploadfile.status == UPLOAD_FILE_END)
  {
    if(ErrorUpdate == 1){
      buf[0] = 0xFF;
      buf[1] = 0xC8;
      datCrcAndSend(&buf[0], 2);
    }
    else{
      //передача завершена 0xC9 4 байта размер данных crc
      buf[0] = 0xC9;
      buf[1] = (uploadfile.totalSize >> 24) & 0xFF;
      buf[2] = (uploadfile.totalSize >> 16) & 0xFF;
      buf[3] = (uploadfile.totalSize >> 8) & 0xFF;
      buf[4] = uploadfile.totalSize & 0xFF;
      datCrcAndSend(&buf[0], 5);
    }
    //Serial.print("UPLOAD_FILE_END:"); Serial.println(uploadfile.currentSize);
    //Serial.print("UPLOAD_FILE_END_SIZE:"); Serial.println(uploadfile.totalSize);
    //handleUpload();
    //if(UploadFile)          // If the file was successfully created
    {                                    
      //UploadFile.close();   // Close the file again
      //Serial.print("Upload Size: "); Serial.println(uploadfile.totalSize);
      webpage = "";
      append_page_header();
      webpage += F("<h2 align=center><br>WEB Server File Upload</h2>");
      if(ErrorUpdate == 1){
        webpage += F("<h3>File was not uploaded</h3>"); 
        webpage += F("<br>"); 
        webpage += F("<p align=center><input type=button value='Return' style='margin-left:20px' onclick=\"location.href='/index.htm';\"/></p>");
        webpage += F("</td></tr></tbody></table></font></div></td></tr></tbody></table></div></body></html>");
      }
      else{
        webpage += F("<h3>File was successfully uploaded</h3>"); 
        webpage += F("<h2>Uploaded File Name: "); webpage += uploadfile.filename+"</h2>";
        webpage += F("<h2>File Size: "); webpage += file_size(uploadfile.totalSize) + "</h2><br>"; 
        webpage += F("<p align=center><input type=button value='Return' style='margin-left:20px' onclick=\"location.href='/index.htm';\"/></p>");
        webpage += F("</td></tr></tbody></table></font></div></td></tr></tbody></table></div></body></html>");
      }
      server.send(200,"text/html",webpage);
      delay(5000);
      StartUpdate = 0;
    } 
  }
}

int calculationCRC(uint8_t* data, int size){
 int j = 0;
 unsigned int crc = 0xFFFF;
 while(size--)
 {
  crc ^= *data++;
  for(j = 0; j < 8; j++)
  {
   if(crc & 0x01)
     crc = (crc >> 1) ^ 0xA001;
   else
     crc = crc >> 1;
  }
 }
 return crc & 0xFFFF;
}

void setup() {
  Serial.begin(115200);

  Serial.println("$");
  //Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  //WiFi.mode(WIFI_AP_STA);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  //WiFi.softAP(ssid, password);
  WiFi.config(ipLocal, ipGateWay, ipMask);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("$.");
  }

  Serial.println("$");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/llblue.jpg", Llblue_gif);
  server.on("/home.png", Home_gif);
  server.on("/network.cgi", LanSetting);
  server.on("/pabb.gif", PabbGif);
  server.on("/index.htm", handleRoot);
  server.on("/network_form", hahdleForm);
  server.on("/form_dat", sendFormDat);
  server.on("/rsParam.cgi", handleRS);
  server.on("/form_dat_rs", sendFormDatRs);
  server.on("/rs_param", handleDatFormRs);
  server.on("/upload.cgi", handleUpload);
  //server.on("/upload_file", fileUpload);
  server.on("/upload_file",  HTTP_POST,[](){ server.send(200);}, handleFileUpload);
  //server.on("/get", IpSet);
 
  server.begin();
  Serial.println("$HTTP server started");

  TCPserver.begin();
  Serial.println("$TCPserver started");
}

void loop() {
  server.handleClient();

 // client = TCPserver.available();
  if(StartUpdate == 1) return;


  //if (millis() - timeout > 5000)
  {
  //if (!client.connect(ipLocal, portServer))
  if (!client.connected())
   {
       client = TCPserver.available();
       //timeout = millis();
       //;
   }
   else
   {
     if(Serial.available()>0){//timeout = millis();
     countByteDat = 0;
      while(Serial.available()>0)    //Checks is there any data in buffer 
       {
        //if (millis() - timeout > 20) break;
        //Serial.print("We got:");
        //Serial.print(char(Serial.read()));  //Read serial data byte and send back to serial monitor
        if(countByteDat < sizeof(buf)) {
          buf[countByteDat++] = Serial.read();
          //Serial.write(buf[countByteDat - 1]);
        }
        delay(1);
       }
      if(countByteDat > 0){
        //if(countByteDat >= 3)
        {
          //crc = calculationCRC(&buf[0], countByteDat - 2);
          //uint16_t crc1 = buf[countByteDat - 2] | ((uint16_t)buf[countByteDat - 1] << 8);
          //if(crc1 == crc)
          //for(int i = 0; i < countByteDat; i++) Serial.write(buf[i]); 
          client.write(&buf[0], countByteDat - 2);
          //Serial.print("$SEND:");
        }
        countByteDat = 0;
        //client.stop();
      }
     }
             
      while (client.available()) {
        //char ch = static_cast<char>(client.read());
        uint8_t ch = client.read();
        //Serial.print(ch);
        datIsReady = 1;
        if(countByteDat < sizeof(buf)) buf[countByteDat] = ch;
        countByteDat++;
        if(countByteDat >= sizeof(buf)) break;
       }
       if(datIsReady == 1){
        datIsReady = 0;
        crc = calculationCRC(&buf[0], countByteDat);
        for(int i = 0; i < countByteDat; i++) Serial.write(buf[i]);
        uint8_t ch = crc & 0xFF;
        Serial.write(ch);
        ch = (crc >> 8) & 0xFF;
        Serial.write(ch);
        countByteDat = 0;
       }
    }
  }
  /********************************************************************************/
}
