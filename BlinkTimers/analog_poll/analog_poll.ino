// ____________________________________________
// ПУНКТ 4: Опрос аналогового пина каждые 250 мс
// Используется своя функция myMillis()
// ____________________________________________

#define LED_PIN 2  
#define ANALOG_PIN A0

volatile unsigned long my_millis_count = 0;
unsigned long previousAnalogMillis = 0;
unsigned long previousBlinkMillis = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  pinMode(ANALOG_PIN, INPUT);
  
  setupTimer1_CustomMillis();
}

void loop() {
  unsigned long currentMillis = myMillis();
  
  if (currentMillis - previousAnalogMillis >= 250) {
    previousAnalogMillis = currentMillis;
    
    int analogValue = analogRead(ANALOG_PIN);
    
    Serial.print(currentMillis);
    Serial.print("      | ");
    Serial.println(analogValue);
  }
  
  if (currentMillis - previousBlinkMillis >= 500) {
    previousBlinkMillis = currentMillis;
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  }
}

void setupTimer1_CustomMillis() {
  cli(); 
  
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  TCCR1B |= (1 << WGM12);

  TCCR1B |= (1 << CS11) | (1 << CS10);
  
  // Для 1 мс: 16000000 / 64 / 1000 - 1 = 249
  OCR1A = 249;
  
  TIMSK1 |= (1 << OCIE1A);
  
  sei(); 
}

ISR(TIMER1_COMPA_vect) {
  my_millis_count++;
}

unsigned long myMillis() {
  unsigned long m;
  uint8_t oldSREG = SREG;
  
  cli();
  m = my_millis_count;
  SREG = oldSREG;
  
  return m;
}