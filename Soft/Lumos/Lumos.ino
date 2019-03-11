#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266SSDP.h>
#include <FS.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoOTA.h>


ESP8266WebServer HTTP;
ESP8266HTTPUpdateServer httpUpdater;
File fsUploadFile;

String _ssid          = "home";
String _password      = "12345678";
String _ssidAP        = "WiFi";
String _passwordAP    = "";
String SSDP_Name      = "Lumos";

String jsonConfig = "{}";
int port = 80;
int timezone = 3;
bool wst;
int md=2; //mode
/*mode: 
1 - static light
2 - sine wave
*/
int sinSpd = 10;
int minVal;
int dir;

//pins
int out[4] = {D3, D4, D5, D6};
int outVal[4] = {};
bool light = 1;

void setup() {
  Serial.begin(115200);
  Serial.println("\n\nStarting");
  for (int p=0; p<4; p++){pinMode(out[p], OUTPUT);}
  analogWriteFreq(10000);
  HTTP = ESP8266WebServer(port);
  FS_init();
  loadConfig();
  WIFI_init();
  SSDP_init();
  HTTP_init();
  OTA_init();
  if(!md) md=1;
  if(!sinSpd) sinSpd=1;
  Serial.println("Ready!");
}

void loop() {
  H();
  if(WiFi.status() != WL_CONNECTED && wst == 1){Reconnect();}
  if(light){
    if(md == 1){ //static light
      H();
      for (int i=0; i<4; i++){analogWrite(out[i], outVal[i]*4);}
    }
    if(md == 2){ //sine wave
      if(dir == 1){
        for(float a=2*PI; a>=0; a=a-0.001*PI){
          for(int i=0; i<4; i++){
            float sa;
            sa = sin(a+i);
            sa = sa*256;
            sa = constrain(sa, minVal, 255);
            outVal[i] = sa;
            analogWrite(out[i], outVal[i]*4);
          }
          if(!light) return;
          if(md!=2) return;
          if(dir!=1) return;
          unsigned long t = millis();
          while(millis()-t<sinSpd){H();}
        }
      }
      if(dir == 0){
        for(float a=0; a<2*PI; a=a+0.001*PI){
          for(int i=0; i<4; i++){
            float sa;
            sa = sin(a+i);
            sa = sa*256;
            sa = constrain(sa, minVal, 255);
            outVal[i] = sa;
            analogWrite(out[i], outVal[i]*4);
          }
          if(!light) return;
          if(md!=2) return;
          if(dir!=0) return;
          unsigned long t = millis();
          while(millis()-t<sinSpd){H();}
        }
      }
    }
  }
  if(!light){
    for(int i=0; i<4; i++){analogWrite(out[i], 0);}
  }
}
