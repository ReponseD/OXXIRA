#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <DHT.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define GAS_SENSOR A0
#define PRESSURE_SENSOR A1
#define TEMP_SENSOR A2
#define VOLTAGE_SENSOR A3
#define RELAY_PIN 7
#define PWM_PIN 9
#define BUZZER 10

Adafruit_BME280 bme;
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo valveServo;

float gasVal, pressureVal, tempVal, voltageVal;
float predictedOutput = 0;
int pwmValue = 0;
unsigned long lastTrigger = 0;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(PWM_PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  valveServo.attach(5);
  digitalWrite(RELAY_PIN, LOW);
  analogWrite(PWM_PIN, 0);
  lcd.begin();
  lcd.backlight();
  dht.begin();
  bme.begin(0x76);
  valveServo.write(0);
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

  float avgTemp = (t + bmeTemp) / 2.0;
  float avgPressure = (pressureVal + bmePressure) / 2.0;

  predictedOutput = (avgTemp * 0.6) + (gasVal * 0.4) - (voltageVal * 0.2);
  if (predictedOutput < 0) predictedOutput = 0;
  if (predictedOutput > 255) predictedOutput = 255;
  pwmValue = (int)predictedOutput;

  analogWrite(PWM_PIN, pwmValue);

  if (gasVal > 750 || avgTemp > 65 || voltageVal > 900) {
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(BUZZER, HIGH);
    valveServo.write(90);
  } else {
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(BUZZER, LOW);
    valveServo.write(30);
  }

  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(avgTemp, 1);
  lcd.print(" H:");
  lcd.print(h, 0);

  lcd.setCursor(0, 1);
  lcd.print("G:");
  lcd.print(gasVal, 0);
  lcd.print(" P:");
  lcd.print(avgPressure, 0);

  Serial.print("TEMP:");
  Serial.print(avgTemp);
  Serial.print(",HUM:");
  Serial.print(h);
  Serial.print(",GAS:");
  Serial.print(gasVal);
  Serial.print(",PRES:");
  Serial.print(avgPressure);
  Serial.print(",PWM:");
  Serial.println(pwmValue);

  delay(1000);
}
