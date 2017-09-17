#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
  *****************************************************************************************
  **** Wifi AP parameters
  *****************************************************************************************/
// WiFi AP parameters
char* ssid = "your_wifi_ssid";
char* password = "your_wifi_password";      //You can freely change it ;)

/*
  *****************************************************************************************
  **** Ubidots configuration
  *****************************************************************************************/
#define TOKEN           "6ZgGCH1FTFDp8ViZ2zaBsfcSsAF7bY"
#define TEMPERATURE_ID  "597160f37625421202d07932"
#define VOLTAGE_ID      "5971616c762542120cea6c34"
#define RELAY_ID        "597162ae7625421202d092e8"
#define SETUP_ID        "5971641976254212005bf66e"


/*
  *****************************************************************************************
  **** DHT11 configuration
  *****************************************************************************************/
//Configuration for DHT11
#define DHT_PIN 4        // Digital Pin for the DHT temperature sensor

#define DHTTYPE DHT22   // DHT 22

/*
  *****************************************************************************************
  **** Relay configuration
  *****************************************************************************************/
#define RELAY_PIN 5     // Digital pin for the Power Relay


#endif

