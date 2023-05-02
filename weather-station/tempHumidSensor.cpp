#include "tempHumidSensor.hpp"

sakurajin::tempHumidSensor::tempHumidSensor(unsigned int pinNum, sensorTypes type):underlyingSensor{pinNum, type}{

  underlyingSensor.begin();

  sensor_t sensorHumid;
  underlyingSensor.humidity().getSensor(&sensorHumid);
  readDelayMicroSeconds = static_cast<uint64_t>(sensorHumid.min_delay);

  sensors_event_t event;
  underlyingSensor.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    return;
  }
  humidityValues = averageValue<double, 10>{event.relative_humidity};

  underlyingSensor.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    return;
  }
  temperatureValues = averageValue<double, 10>{event.temperature};

  nextReadTime = micros() + readDelayMicroSeconds;
}

void sakurajin::tempHumidSensor::printSensorInfo(){
  sensor_t sensor;

  // Print temperature sensor details.
  underlyingSensor.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));

  // Print humidity sensor details.
  underlyingSensor.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
}

void sakurajin::tempHumidSensor::updateValues(bool skipUnready){
  uint64_t currTime = micros();
  if(currTime < nextReadTime){
    if(skipUnready){return;}

    auto diff = nextReadTime - currTime;
    delayMicroseconds(diff);
  }

  sensors_event_t event;
  underlyingSensor.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
    return;
  }
  humidityValues << event.relative_humidity;

  underlyingSensor.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
    return;
  }
  temperatureValues << event.temperature;

  nextReadTime = currTime + readDelayMicroSeconds;

}

uint64_t sakurajin::tempHumidSensor::getMicrosDelay() const{
  return readDelayMicroSeconds;
}

double sakurajin::tempHumidSensor::temperature() const{
  return temperatureValues.average();
}

double sakurajin::tempHumidSensor::humidity() const{
  return humidityValues.average();
}

double sakurajin::tempHumidSensor::latestTemperature() const{
  return temperatureValues[9];
}

double sakurajin::tempHumidSensor::latestHumidity() const{
  return humidityValues[9];
}
