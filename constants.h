#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
  *****************************************************************************************
  **** Wifi AP parameters
  *****************************************************************************************/
// WiFi AP parameters
const char* ssid = "ESP8266-Endesa-IOT";
const char* password = "OscarNeira";      //You can freely change it ;)

/*
  *****************************************************************************************
  **** Thingspeak configuration
  *****************************************************************************************/
const char* HOST = "api.thingspeak.com";
//For validation of HOST and avoid MITM attacks, the following fingerprint is checked against
//the certificate showd by the above HOST using secure connection
const char* FINGERPRINT =  "78 60 18 44 81 35 bf df 77 84 d4 0a 22 0d 9b 4e 6c dc 57 2c"; 
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

/*
  *****************************************************************************************
  **** DHT11 configuration
  *****************************************************************************************/
//Configuration for DHT11
#define DHT_PIN 4        // Digital Pin for the DHT temperature sensor

#define DHTTYPE DHT11   // DHT 11

/*
  *****************************************************************************************
  **** Relay configuration
  *****************************************************************************************/
#define RELAY_PIN 10     // Digital pin for the Power Relay


#endif

