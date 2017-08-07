#include <DHT.h>              // Needs Adafruit Unified Sensor library installed
#include <ESP8266WiFi.h>
//#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
//#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
//#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <UbidotsMicroESP8266.h>  //https://github.com/ubidots/ubidots-esp8266 ubidots library

#include "constants.h"            //API keys & WiFi SSID and Passwords are defined here
#define SECONDS_DEEPSLEEP 300    //Seconds in deep sleep

DHT dht(DHT_PIN, DHTTYPE, 11);    //Third parameter thanks to http://homecircuits.eu/blog/esp8266-temperature-iot-logger/
float temperature = 0;

ADC_MODE(ADC_VCC);      // Required being able to read input voltage of ESP8266
float voltage = 0;

//WiFiManager wifiManager;

Ubidots * UbidotsClient;

long startTime;

extern "C" {
  #include "user_interface.h"
}

// returns -1 on error
int getResetReason() {
  rst_info* ri = system_get_rst_info();
  if (ri == NULL)  
    return -1;

  return ri->reason;
} 

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

void setup() {
  // put your setup code here, to run once:
  startTime = millis();
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);           // set pin to output
  pinMode(DHT_PIN, INPUT_PULLUP);       // set pin to input

  if (getResetReason()!=5){
    digitalWrite(RELAY_PIN, LOW);
    Serial.printf("\nReason for restart: %d\n", getResetReason());    
  }else{
    Serial.println("Waking up from DeepSleep");
  }
  dht.begin();
  delay(100);  //para dar tiempo a que el DHT se normalice
  temperature = dht.readTemperature();
  if (isnan(temperature)){
    dht.begin();
    delay(100);
    temperature = dht.readTemperature();
  }
  
  voltage = ESP.getVcc();
  Serial.printf("\nTemperature %f degrees Celsius\n", temperature);
  UbidotsClient = new Ubidots(TOKEN);
  UbidotsClient->setSSL(true);
  UbidotsClient->setDebug(true);
  UbidotsClient->wifiConnection(ssid, password);
  Serial.println("");
  Serial.println("Connected to WiFi Network");

}


void loop() {
  // put your main code here, to run repeatedly:
    //Read variables and send them to cloud server
    float setupval;
    if (!isnan(temperature)){
      setupval = UbidotsClient->getValue(SETUP_ID);  //In order for ubidots to calculate correct relay status, this variable has to be updated
      UbidotsClient->add(TEMPERATURE_ID,temperature);
      UbidotsClient->add(VOLTAGE_ID, voltage);
      UbidotsClient->add(SETUP_ID, setupval);
      UbidotsClient->sendAll();
      Serial.println("Data sent to server");
    }
    float relay_value;
    relay_value = UbidotsClient->getValue(RELAY_ID);
    digitalWrite(RELAY_PIN, relay_value > 0);
    digitalWrite(RELAY_PIN, HIGH);

    Serial.printf("\nLoop duration %d seconds. Now entering deep sleep\n", (millis()-startTime)/1000 );
    ESP.deepSleep(SECONDS_DEEPSLEEP * 1000 * 1000, WAKE_RF_DEFAULT);
}
