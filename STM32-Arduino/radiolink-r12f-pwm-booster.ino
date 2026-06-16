/*
 * RadioLink R12F RC PWM (%7 - %14) -> Endüstriyel Motor Sürücü PWM (%0-%100) Dönüştürücü
 */

const int aliciPin = PA0;   // R12F CH4 Girişi
const int surucuPin = PA6;  // Motor Sürücü PWM Çıkışı

// Sizin ölçtüğünüz %7 ve %14 limitlerinin tam mikro saniye karşılıkları:
const unsigned long RC_MIN = 1000; // %7 alt sınırı (1000 mikrosaniye)
const unsigned long RC_MAX = 2000; // %14 üst sınırı (2000 mikrosaniye)

void setup() {
  pinMode(aliciPin, INPUT);
  pinMode(surucuPin, OUTPUT);
  
  // 1 kHz frekansta tam kare dalga üretebilmek için 8-bit çözünürlük en kararlısıdır.
  analogWriteResolution(8);  
  analogWriteFrequency(1000); // 1 kHz endüstriyel standart frekans
}

void loop() {
  unsigned long duration = pulseIn(aliciPin, HIGH, 30000); 
  
  // Emniyet: Sinyal koptuğunda motoru anında durdur (%0 yap)
  if (duration == 0) {
    digitalWrite(surucuPin, LOW);
    return;
  }
  
  // --- KESİN %0 VE %100 DIJITAL KİLİTLEME ---
  // Kumanda kolu en alttaysa (%7 / 1000us civarı) PWM'i kapat ve pini sıfıra çek (Tam %0)
  if (duration <= (RC_MIN + 30)) {
    digitalWrite(surucuPin, LOW); 
    return;
  }
  
  // Kumanda kolu en üstteyse (%14 / 2000us civarı) PWM'i kapat ve pini HIGH yap (Tam %100)
  if (duration >= (RC_MAX - 30)) {
    digitalWrite(surucuPin, HIGH); 
    return;
  }
  
  // Ara değerlerde taşmaları önlemek için sinyali sınırla
  if (duration < RC_MIN) duration = RC_MIN;
  if (duration > RC_MAX) duration = RC_MAX;
  
  // %0 ile %100 arasında (0 - 255) doğrusal haritalama yap
  int pwmCikis = map(duration, RC_MIN, RC_MAX, 0, 255); 
  analogWrite(surucuPin, pwmCikis);
  
  delay(5); 
}
