// --- TB6600 Step Motor Driver ---
// 3200 pulse/rev 16 microstep  OFF OFF ON 
// 0.5mA Current for MOONS 16HY4401-02N Motors

const int PUL = 9;
const int DIR = 8;

const int MICROSTEP = 16;                 
const long STEPS_PER_REV = 200L * MICROSTEP;
const int TURNS = 2;                      // her yönde kaç tur
const int STEP_US = 100;                  // darbe yarım periyodu (µs)

void setup() {
  pinMode(PUL, OUTPUT);
  pinMode(DIR, OUTPUT);
}

void doSteps(long steps, int us) {
  for (long i = 0; i < steps; i++) {
    digitalWrite(PUL, HIGH);
    delayMicroseconds(us);
    digitalWrite(PUL, LOW);
    delayMicroseconds(us);
  }
}

void loop() {
  digitalWrite(DIR, HIGH);
  doSteps(STEPS_PER_REV * TURNS, STEP_US);
  delay(1000);

  digitalWrite(DIR, LOW);
  doSteps(STEPS_PER_REV * TURNS, STEP_US);
  delay(1000);
}
