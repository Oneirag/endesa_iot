#include <DHT.h>              // Needs Adafruit Unified Sensor library installed
#include <ESP8266WiFi.h>
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include "constants.h"

#define DEBUG 1                   //For printing debug messages

float temperature = 0;
DHT dht(DHT_PIN, DHTTYPE);

ADC_MODE(ADC_VCC);      // Required being able to read input voltage of ESP8266
float voltage = 0;

WiFiManager wifiManager;

//bool connect_to_wifi(){
//  //sets timeout until configuration portal gets turned off
//  //useful to make it all retry or go to sleep
//  //in seconds
//  wifiManager.setTimeout(300);
//
//  //fetches ssid and pass and tries to connect
//  //if it does not connect it starts an access point with the specified name
//  //and goes into a blocking loop awaiting configuration
//  if(!wifiManager.autoConnect(ssid, password)) {
//    Serial.println("failed to connect and hit timeout");
//    return false;
//  }else{
//    return true;
//  }
//    
//}

bool connect_to_wifi(){

  WiFi.mode(WIFI_STA);
  WiFi.begin("NeiraPinuela", "123456");
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi Network");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
}

int nErrores = 0;
WiFiClientSecure client;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  connect_to_wifi();
}

bool connect_to_secureserver(const char* host, const char* fingerprint){
  #if DEBUG
  Serial.print("connecting to " + String(host));
  #endif
  int httpsPort = 443;
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return false;
  }
  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
    return false;
  }
  
}

bool write_thingspeak(int field, float &value, String api_key){
    if (connect_to_secureserver(HOST, FINGERPRINT)){ 
      String url = "/update?api_key=" + String(api_key) + "&field" + String(field) + "=" + String(value);
      #if DEBUG 
        Serial.println(url);
      #endif
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
             "Host: " + HOST + "\r\n" +
             "User-Agent: EndesaESP8266-IoT\r\n" +
             "Connection: close\r\n\r\n");
    }else{
      Serial.println("Error connecting to " + String(HOST));      
    }
}

void loop() {
  // put your main code here, to run repeatedly:
//  if (connect_to_wifi()){
    //Read variables and send them to thingspeak
    temperature = dht.readTemperature();
    Serial.println("En loop"); delay(100);
    temperature = dht.readTemperature();
    voltage = ESP.getVcc();
    Serial.println("Leido voltage"); delay(100);
    delay(100);
    write_thingspeak(2, voltage, myWriteAPIKey);
    delay(16000);
    write_thingspeak(1, temperature, myWriteAPIKey);

    //ThingSpeak.writeField(myChannelNumber, 1, temperature, myWriteAPIKey);
    Serial.println("Enviado temperature"); delay(100);
    //ThingSpeak.writeField(myChannelNumber, 2, voltage, myWriteAPIKey);
    Serial.println("Enviado voltage"); delay(100);
    float relayStatus = 1;
    //float relayStatus = ThingSpeak.readFloatField(RelayChannelNumber, 1, RelayReadAPIKey);
    Serial.println("leido status"); delay(100);
    
    if (relayStatus>0){
      digitalWrite(RELAY_PIN, HIGH);
    }else{
      digitalWrite(RELAY_PIN, LOW);
    }
    Serial.println("Conectado");
//  }
//  else{
//    {
//      Serial.println("Error de conexión");
//      nErrores++;
//      if (nErrores>10){
//        wifiManager.resetSettings();
//        nErrores = 0;
//    }
//  }
  delay(20000);

}
