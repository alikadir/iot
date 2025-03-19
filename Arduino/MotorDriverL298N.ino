#define EN1 10

#define IN1 9
#define IN2 8
#define IN3 7
#define IN4 6

#define EN2 5


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(EN1, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(EN2, OUTPUT);
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  goFast();
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);
  goSlow();
  delay(5000);

  digitalWrite(LED_BUILTIN, HIGH);
  backFast();
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);
  backSlow();
  delay(5000);
}

void goSlow() {
  analogWrite(EN1, 20);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  analogWrite(EN2, 20);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void goFast() {
  analogWrite(EN1, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  analogWrite(EN2, 255);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void backSlow() {
  analogWrite(EN1, 20);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  analogWrite(EN2, 20);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void backFast() {
  analogWrite(EN1, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  analogWrite(EN2, 255);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
