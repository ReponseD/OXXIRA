void setup() {
  Serial.begin(9600);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
}

void loop() {
  int gas = analogRead(A0);
  int temp = analogRead(A1);
  int water = analogRead(A2);
  Serial.print(gas);
  Serial.print(",");
  Serial.print(temp);
  Serial.print(",");
  Serial.println(water);
  delay(500);
}

