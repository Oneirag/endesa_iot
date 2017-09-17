#include <UbidotsMicroESP8266.h>  //https://github.com/ubidots/ubidots-esp8266 ubidots library
#include <DHT.h>              // Needs Adafruit Unified Sensor library installed

#include "constants.h"            //API keys & WiFi SSID and Passwords are defined here
#define SECONDS_DEEPSLEEP 300    //Seconds in deep sleep

DHT dht(DHT_PIN, DHTTYPE, 11);    //Third parameter thanks to http://homecircuits.eu/blog/esp8266-temperature-iot-logger/
float temperature = 0;

ADC_MODE(ADC_VCC);      // Required being able to read input voltage of ESP8266
float voltage = 0;

Ubidots * UbidotsClient;
long startTime;

void setup() {
  // put your setup code here, to run once:
  startTime = millis();
  Serial.begin(115200);
  dht.begin();
  temperature = dht.readTemperature();
  if (isnan(temperature)){      // It might need a retry 
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
      UbidotsClient->add(TEMPERATURE_ID, temperature);
      UbidotsClient->add(VOLTAGE_ID, voltage);
      UbidotsClient->add(SETUP_ID, setupval);
      UbidotsClient->sendAll();
      Serial.println("Data sent to server");
    }
    //Now read relay status from cloud server and update relay accordingly 
    float relay_value;
    relay_value = UbidotsClient->getValue(RELAY_ID);
    digitalWrite(RELAY_PIN, relay_value > 0);
    digitalWrite(RELAY_PIN, HIGH);

    //Finalize loop and deep sleep to save battery
    Serial.printf("\nLoop duration %d seconds. Now entering deep sleep\n", (millis()-startTime)/1000 );
    ESP.deepSleep(SECONDS_DEEPSLEEP * 1000 * 1000, WAKE_RF_DEFAULT);
}
