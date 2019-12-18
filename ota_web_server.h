#ifndef OTA_WEB_SERVER_H
#define OTA_WEB_SERVER_H
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <HTTPClient.h>
#include <ESP32httpUpdate.h>
#define USE_SERIAL Serial
class ota_web_server{
  private:
        char* userTopic;
        bool statusStart = false;
  public:
        ota_web_server(char* userTopic);
        ~ota_web_server();
        const char* mqtt_server = "150.95.31.161";
        const int mqtt_port = 1883;
        const char* MQTT_USER = "admin";
        const char* MQTT_PASSWORD = "admin";
        char* mqtt_status_publish = userTopic;
        
        String msg;
        bool setupStart(bool statusStart = false);
        void loop();
    
  
};
#endif
