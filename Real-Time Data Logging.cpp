#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DHT.h>

#define GAS_SENSOR A0
#define PRESSURE_SENSOR A1
#define TEMP_SENSOR A2
#define VOLTAGE_SENSOR A3

Adafruit_BME280 bme;
DHT dht(2, DHT22);

float gasVal, pressureVal, tempVal, voltageVal;
float temp, humidity, bmeTemp, bmePressure;

void setup() {
  Serial.begin(9600);
  dht.begin();
  bme.begin(0x76);
}

void loop() {
  gasVal = analogRead(GAS_SENSOR);
  pressureVal = analogRead(PRESSURE_SENSOR);
  tempVal = analogRead(TEMP_SENSOR);
  voltageVal = analogRead(VOLTAGE_SENSOR);

  temp = dht.readTemperature();
  humidity = dht.readHumidity();
  bmeTemp = bme.readTemperature();
  bmePressure = bme.readPressure() / 100.0F;

  Serial.print("Gas Value: ");
  Serial.print(gasVal);
  Serial.print(", Pressure Value: ");
  Serial.print(pressureVal);
  Serial.print(", Temp: ");
  Serial.print(bmeTemp);
  Serial.print(", BME Pressure: ");
  Serial.print(bmePressure);
  Serial.print(", Voltage: ");
  Serial.println(voltageVal);

  delay(1000);
}
