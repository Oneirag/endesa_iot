#include <DHT.h>              // Needs Adafruit Unified Sensor library installed
// Import required libraries
#include <ESP8266WiFi.h>
// Import required libraries
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
//#include <ThingSpeak.h>
// WiFi AP parameters
const char* ssid = "ESP8266-Endesa-IOT";
const char* password = "OscarNeira";
const char* HOST = "api.thingspeak.com";
const char* FINGERPRINT =  "78 60 18 44 81 35 bf df 77 84 d4 0a 22 0d 9b 4e 6c dc 57 2c";

float temperature = 0;
#define DHT_PIN 4        // Digital Pin for the DHT temperature sensor
#define RELAY_PIN 4     // Digital pin for the Power Relay

#define DHTTYPE DHT11   // DHT 11
DHT dht(DHT_PIN, DHTTYPE);

ADC_MODE(ADC_VCC);      // Required being able to read input voltage of ESP8266
float voltage = 0;
/*
  *****************************************************************************************
  **** Visit https://www.thingspeak.com to sign up for a free account and create
  **** a channel.  The video tutorial http://community.thingspeak.com/tutorials/thingspeak-channels/ 
  **** has more information. You need to change this to your channel, and your write API key
  **** IF YOU SHARE YOUR CODE WITH OTHERS, MAKE SURE YOU REMOVE YOUR WRITE API KEY!!
  *****************************************************************************************/
unsigned long myChannelNumber = 25159;
const char * myWriteAPIKey = "U0C3D7AHYOYPC86R";
unsigned long RelayChannelNumber = 296411;
const char * RelayReadAPIKey = "NDC713BNFZHKTUUM";

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
  //WiFi.begin("NeiraPinuela", "Pablo&Carlos");
  WiFi.begin("ONO573C", "Pablo13Carlos16");
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
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
  Serial.print("connecting to ");
  Serial.println(host);
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

bool request_thingspeak(int field, String operation, float &value, String api_key, long channel){
    if (connect_to_secureserver(HOST, FINGERPRINT)){ 
      String url = "";
      if (operation=="update") {
        url = "/" + operation + "?api_key=" + String(api_key) + "&field" + String(field) + "=" + String(value);
        Serial.println(url);
      }
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
             "Host: " + HOST + "\r\n" +
             "User-Agent: BuildFailureDetectorESP8266\r\n" +
             "Connection: close\r\n\r\n");
    }else{
      Serial.println("Error conectando a " + String(HOST));      
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
//    GET https://api.thingspeak.com/update?api_key=U0C3D7AHYOYPC86R&field1=0
    delay(100);
    request_thingspeak(2, "update", voltage, myWriteAPIKey, myChannelNumber);
    delay(16000);
    request_thingspeak(1, "update", temperature, myWriteAPIKey, myChannelNumber);

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
