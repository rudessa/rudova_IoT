// ___________________________________________
// ПУНКТ 2: Своя реализация функции millis()
// Используется Timer1 (16-битный)
// ___________________________________________

#define LED_PIN 2 

volatile unsigned long my_millis_count = 0;
unsigned long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  setupTimer1_CustomMillis();
}

void loop() {
  unsigned long currentMillis = myMillis();
  
  if (currentMillis - previousMillis >= 500) {
    previousMillis = currentMillis;
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    
    Serial.print("myMillis(): ");
    Serial.print(currentMillis);
    Serial.print(" | встроенный millis(): ");
    Serial.println(millis());
  }
}

void setupTimer1_CustomMillis() {
  cli(); // Отключаем прерывания
  
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  TCCR1B |= (1 << WGM12);

  TCCR1B |= (1 << CS11) | (1 << CS10);
  
  // Расчет для 1 мс при частоте 16 МГц:
  // 16000000 / 64 / 1000 = 250 тиков на 1 мс
  // Счет с 0, поэтому: 250 - 1 = 249
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