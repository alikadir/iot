const int ledPin = 3;

float brightness = 75;             // Başlangıç parlaklığı
float targetBrightness = 150;
const float smoothing = 0.07;       // Ne kadar yavaş değişsin
unsigned long lastChange = 0;
unsigned long changeInterval = 100; // Yeni hedef parlaklık ne kadar sürede değişsin (ms)

void setup() {
    pinMode(13, OUTPUT);
  digitalWrite(13, LOW); // LED söner
  
  pinMode(ledPin, OUTPUT);
  randomSeed(analogRead(A0));
}

void loop() {
  unsigned long now = millis();

  // Belirli aralıklarla yeni bir hedef parlaklık üret
  if (now - lastChange > changeInterval) {
    targetBrightness = random(100, 255); // Daha geniş aralıkla gözle fark edilir titreşim
    lastChange = now;
  }

  // Yumuşak geçiş (smoothstep gibi)
  brightness += (targetBrightness - brightness) * smoothing;

  analogWrite(ledPin, (int)brightness);
  delay(20);
}