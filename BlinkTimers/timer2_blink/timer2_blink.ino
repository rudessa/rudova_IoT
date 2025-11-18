// ============================================
// ПУНКТ 1: Blink по прерываниям на 8-битном таймере
// Максимальный период ~2 секунды
// ============================================

#define LED_PIN 2  // Светодиод подключен к пину 2

volatile unsigned int timer2_counter = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  setupTimer2();
}

void loop() {}

void setupTimer2() {
  cli(); 

  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;

  TCCR2A |= (1 << WGM21);

  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);

  OCR2A = 255;

  TIMSK2 |= (1 << OCIE2A);
  
  sei();
  
  // Расчет частоты прерываний:
  // f = 16000000 / (1024 * 256) ≈ 61 Гц
  // Период одного прерывания ≈ 16.4 мс
  // Для периода 2 сек: 2000 / 16.4 ≈ 122 прерывания
  
  Serial.println("[OK] Timer2 настроен:");
  Serial.println("  - Предделитель: 1024");
  Serial.println("  - OCR2A: 255");
  Serial.println("  - Частота прерываний: ~61 Гц");
  Serial.println("  - Период мигания: ~2 сек");
  Serial.println();
}

ISR(TIMER2_COMPA_vect) {
  timer2_counter++;

  if (timer2_counter >= 122) {
    timer2_counter = 0;

    PORTD ^= (1 << 2);
  }
}