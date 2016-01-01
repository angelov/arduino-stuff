#include "DHT.h"
#include <math.h>

DHT dht(2, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();
  delay(1000);
}

float readTemperatureFromAnalog(int pin) {
  // http://www.learningaboutelectronics.com/Articles/LM335-temperature-sensor-circuit.php
  int rawvoltage = analogRead(A0);
  float millivolts = (rawvoltage / 1024.0) * 5000;
  float kelvin = (millivolts / 10);

  float celsius = kelvin - 273.15;
  celsius = round(celsius);

  return celsius;
}

void loop() {
  delay (3000);

  float celsius = readTemperatureFromAnalog(A0);

  Serial.print("Analog A0: ");
  Serial.println(celsius);

  celsius = readTemperatureFromAnalog(A1);

  Serial.print("Analog A1: ");
  Serial.println(celsius);

  Serial.print("Digital: ");
  Serial.println(dht.readTemperature());

  Serial.println("----------");
}
