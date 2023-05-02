#include "Arduino.h"
#include "tempHumidSensor.hpp"

using namespace sakurajin;

tempHumidSensor* dhtSensor;

void setup() {
  Serial.begin(19200);
  dhtSensor = new tempHumidSensor(26, dht22);
}


void loop() {
  dhtSensor->updateValues();

  auto temp = dhtSensor->temperature();
  Serial.print("Temperature: ");
  Serial.println(temp);
  //displayFloat(temp,1,1);
  //control.setChar(1,7,'t',false);

  //get and display the humidity in the bottom row
  auto humid = dhtSensor->humidity();
  Serial.print(F("Humidity: "));
  Serial.println(humid);
  //displayFloat(humid,0,1);
  //control.setChar(0,7,'H',false);
}
