#include "Arduino.h"

#include "tempHumidSensor.hpp"
#include "segmentDisplay.hpp"

using namespace sakurajin;

tempHumidSensor* dhtSensor;
segmentDisplay<2>* segDisplay;

void setup() {
  Serial.begin(19200);
  dhtSensor = new tempHumidSensor(26, dht22);

  controller_configuration<1,2> conf;
  conf.cs_pin = 0;
  conf.virtual_multi_row = false;
  conf.row_cs_pin[0] = 33;
  conf.row_cs_pin[1] = 14;
  conf.useHardwareSpi = false;
  conf.clk_pin = 32;
  conf.mosi_pin = 25;
  conf.debug_output = true;
  conf.spiTransferSpeed = 600000;
  segDisplay = new segmentDisplay<2>(conf);

}


void loop() {
  dhtSensor->updateValues();

  auto temp = dhtSensor->temperature();
  Serial.print("Temperature: ");
  Serial.println(temp);
  segDisplay->display(temp,1,1);
  segDisplay->controller().setChar(1,7,'t',false);

  //get and display the humidity in the bottom row
  auto humid = dhtSensor->humidity();
  Serial.print(F("Humidity: "));
  Serial.println(humid);
  segDisplay->display(humid,0,1);
  segDisplay->controller().setChar(0,7,'H',false);
}
