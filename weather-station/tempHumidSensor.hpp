#pragma once

#include "average-value.hpp"

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

namespace sakurajin{
  enum sensorTypes{
    dht11 = DHT11, //DHT 11
    dht22 = DHT22, //DHT 22 (AM2302)
    dht21 = DHT21 //DHT 21 (AM2301)
  };

  class tempHumidSensor{
    private:
      DHT_Unified underlyingSensor;

      uint64_t nextReadTime;
      uint64_t readDelayMicroSeconds;

      averageValue<double, 10> temperatureValues;
      averageValue<double, 10> humidityValues;

    public:
      tempHumidSensor(unsigned int pinNum, sensorTypes type);
      void printSensorInfo();
      void updateValues(bool skipUnready = false);

      uint64_t getMicrosDelay() const;
      double latestTemperature() const;
      double latestHumidity() const;
      double temperature() const;
      double humidity() const;
  };
}
