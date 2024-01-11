#define INTERNAL_LED 2

void setup() {
  pinMode(INTERNAL_LED,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  digitalWrite(INTERNAL_LED,HIGH);
  Serial.println("LED ON");
  delay(1000);
  digitalWrite(INTERNAL_LED,LOW);
  Serial.println("LED OFF");
  delay(1000);
}
