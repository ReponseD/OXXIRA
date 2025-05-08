#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>

#define PRESSURE_SENSOR A0
#define TEMP_SENSOR A1
#define GAS_SENSOR A2
#define VOLTAGE_SENSOR A3
#define LED_PIN 13
#define BUZZER_PIN 8
#define RELAY_PIN 7
#define PWM_PIN 9

Adafruit_BME280 bme;
DHT dht(2, DHT22);
LiquidCrystal_I2C lcd(0x27, 16, 2);

float gasVal, pressureVal, tempVal, voltageVal;
float maxPressure = 1020;  
float minPressure = 980;
float maxTemp = 75;
float minVoltage = 10.5;
int pwmValue = 0;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  lcd.begin();
  lcd.backlight();
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

  pwmValue = map(avgPressure, minPressure, maxPressure, 0, 255);

  analogWrite(PWM_PIN, pwmValue);

  if (avgPressure < minPressure || avgPressure > maxPressure || avgTemp > maxTemp || voltageVal < minVoltage) {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
  }

  lcd.setCursor(0, 0);
  lcd.print("P:");
  lcd.print(avgPressure);
  lcd.print("T:");
  lcd.print(avgTemp);

  lcd.setCursor(0, 1);
  lcd.print("G:");
  lcd.print(gasVal);
  lcd.print("V:");
  lcd.print(voltageVal);

  delay(1000);
}
