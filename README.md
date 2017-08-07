# endesa_iot

Simple demostration code for a DIY IOT thermostat using ESP8266, DHT11, a relay board and ThingSpeak
![Simple thermostat](fritzing/circuit_batteries.png?raw=true "Thermostat scheme")


### Prerequisites

Hardware: 
* ESP8266-201 Board
* DHT11 temperature sensor
* A Relay
* USB to TTL board to connect to PC
* Breadboard
* Jumper wires

Software:
* Arduino 1.8.2
* ESP8266 library for Arduino
* DHT11 library

An account in ThingSpeak. 


## Deployment

Add additional notes about how to deploy this on a live system

## Built With

* [Arduino IDE 1.8.2](http://www.dropwizard.io/1.0.2/docs/) - The web framework used
* [ESP8266 library for Arduino](https://github.com/esp8266/Arduino) - For programing ESP8266 with Arduino IDE >2.3.0 (If no newer version than 2.3.0 available, download and install latest version from github) 
* [Adafruit DHT11 Sensor library] - For using DHT11. Instalable inside Arduino IDE
* [Adafruit Unified Sensor library] - For using DHT11. Instalable inside Arduino IDE
* [WiFi Manager](https://github.com/tzapu/WiFiManager) - For autoconfigure WiFi Connection (Optional)
* [ThingSpeak](https://www.thingspeak.com) - Cloud service for writing and reading feeds (Temperature and voltage)
* [Frizting](http://www.fritzing.org/) - Used to generate nice charts of the circuit located in the fritzing directory(Optional)


## Author

* **Oscar Neira** - *Initial work* - [Oscar Neira](https://github.com/oneirag)


## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* My wife
* My two kids

