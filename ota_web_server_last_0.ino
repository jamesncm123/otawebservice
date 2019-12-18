/**
 * httpUpdate.ino
 *
 *  Created on: 27.11.2015
 *
 */
#include "ota_web_server.h"
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
WiFiClient wifiClient;
PubSubClient client(wifiClient);


//ใส้ userTopic ที่ได้จาก web
char* userTopic = "jamesncm/1.6"; 
//ota
ota_web_server ota(userTopic);
// กำหนด wifi
const char* ssid = "OPPO"; // ชื่อ wifi
const char* password = "12345678"; // รหัส wifi
///////////////////////////////////////////
const char* mqtt_server = ota.mqtt_server;
const int mqtt_port = ota.mqtt_port;
const char* MQTT_USER = ota.MQTT_USER;
const char* MQTT_PASSWORD  = ota.MQTT_PASSWORD;
char* mqtt_status_publish = ota.mqtt_status_publish;
//////////////////////////////////////////
unsigned long interval = 30000;
unsigned long _next =  millis() + interval;

void setup() {

  pinMode(2,OUTPUT);
  ////// setup ///////////////////////////////////////
    Serial.begin(115200);
    USE_SERIAL.begin(115200);
    // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();
    //// นับถอยหลัง 5 วินาที
    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
  
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Version = jamesncm/1.6.0");
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    reconnect();
   //--------------------------------------------------//
}

void loop() {
    //////
    client.loop();
    ota.loop();
    // coding chill // 
        unsigned long  t = millis();
    if(t >= _next){
    //Serial.println(a);
    //Serial.println("sentMqtt");
      char statusMsg[50] = "";
      String statusString ="";
      statusString += userTopic;
      statusString += "/running";
      statusString.toCharArray(statusMsg,50);
      client.publish("status", statusMsg);
    _next = t + interval;
   
    }
    digitalWrite(2,LOW);
    delay(1000);
    digitalWrite(2,HIGH);
    delay(1000);
    

    
}

void callback(char* topic, byte *payload, unsigned int length) {
    Serial.println("-------new message from broker-----");
    Serial.print("channel:");
    Serial.println(topic);
    //topics = topic;
    
    Serial.print("data:");  
    Serial.write(payload, length);
    Serial.println();
    for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    ota.msg += (char)payload[i];
  }
  
  Serial.println();
  if(String(topic) == userTopic){
    ota.setupStart(true);
    
    Serial.println(ota.msg);
   
  }
}

void reconnect(){
  
   // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),MQTT_USER,MQTT_PASSWORD)) {
      Serial.println("connected");
      //Once connected, publish an announcement...
      client.publish(userTopic, "running");
      // ... and resubscribe
      client.subscribe(userTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
  
}
