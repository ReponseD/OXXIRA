#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DHT.h>

#define PRESSURE_SENSOR A0
#define TEMP_SENSOR A1
#define GAS_SENSOR A2
#define VOLTAGE_SENSOR A3
#define RELAY_PIN 7
#define BUZZER_PIN 8
#define LED_PIN 13

Adafruit_BME280 bme;
DHT dht(2, DHT22);

float gasVal, pressureVal, tempVal, voltageVal;
float minPressure = 1000;
float maxTemp = 75;
float minVoltage = 10.5;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
  bme.begin(0x76);
  Serial.begin(9600);
}

void loop() {
  gasVal = analogRead(GAS_SENSOR);
  pressureVal = analogRead(PRESSURE_SENSOR);
  tempVal = analogRead(TEMP_SENSOR);
  voltageVal = analogRead(VOLTAGE_SENSOR);

  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float bmeTemp = bme.readTemperature();
  float bmePressure = bme.readPressure() / 100.0F;

  float avgPressure = (pressureVal + bmePressure) / 2.0;
  float avgTemp = (t + bmeTemp) / 2.0;

  if (avgPressure < minPressure || avgTemp > maxTemp || voltageVal < minVoltage) {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    delay(3000);  
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    Serial.println("System Shutdown due to anomaly.");
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  Serial.print("Pressure: ");
  Serial.print(avgPressure);
  Serial.print(", Temp: ");
  Serial.print(avgTemp);
  Serial.print(", Voltage: ");
  Serial.print(voltageVal);
  Serial.println();

  delay(1000);
}
