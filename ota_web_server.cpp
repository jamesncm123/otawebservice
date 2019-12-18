#include "ota_web_server.h"

ota_web_server::ota_web_server(char* userTopic){
  this->userTopic= userTopic;
}
ota_web_server::~ota_web_server(){
  
}
bool ota_web_server::setupStart(bool statusStart){
  this->statusStart = statusStart;
  return this->statusStart;
}
void ota_web_server::loop(){
  if(statusStart){
    if((WiFi.status() == WL_CONNECTED)) {
        String part = "http://taisolution.tech/public_html/OTA/"+this->msg;
        Serial.println(part);
        t_httpUpdate_return ret = ESPhttpUpdate.update(part);

        switch(ret) {
            case HTTP_UPDATE_FAILED:
                USE_SERIAL.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                break;

            case HTTP_UPDATE_NO_UPDATES:
                USE_SERIAL.println("HTTP_UPDATE_NO_UPDATES");
                break;

            case HTTP_UPDATE_OK:
                USE_SERIAL.println("HTTP_UPDATE_OK");
                break;
        }
    }
  }
}
