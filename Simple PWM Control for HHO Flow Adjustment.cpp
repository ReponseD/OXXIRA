#define PWM_PIN 9
#define GAS_SENSOR A0
#define PRESSURE_SENSOR A1

int gasVal, pressureVal;
int pwmVal = 0;

void setup() {
  pinMode(PWM_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  gasVal = analogRead(GAS_SENSOR);
  pressureVal = analogRead(PRESSURE_SENSOR);

  pwmVal = map(pressureVal, 0, 1023, 0, 255);
  analogWrite(PWM_PIN, pwmVal);

  Serial.print("Gas: ");
  Serial.print(gasVal);
  Serial.print(" Pressure: ");
  Serial.print(pressureVal);
  Serial.print(" PWM: ");
  Serial.println(pwmVal);

  delay(1000);
}
