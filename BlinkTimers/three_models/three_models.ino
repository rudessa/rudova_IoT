// _________________________________________________
// ПУНКТ 3: Три вида Blink
// 1. По прерыванию (всегда активен) - Timer2
// 2. Режим 1 - два LED (переключаемый через UART)
// 3. Режим 2 - три LED (переключаемый через UART)
// __________________________________________________

#define LED_PIN 2 

volatile unsigned int timer2_counter = 0;
volatile unsigned long my_millis_count = 0;

enum BlinkMode {
  MODE_INTERRUPT,  
  MODE_1,          
  MODE_2           
};

BlinkMode currentMode = MODE_INTERRUPT; 

unsigned long previousMillis = 0;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(9600);
  Serial.println();
  
  setupTimer2_Interrupt();
  setupTimer1_CustomMillis();
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    while(Serial.available()) Serial.read();
    
    if (command == '1') {
      currentMode = MODE_1;
      previousMillis = myMillis(); 
      Serial.println("✓ РЕЖИМ 1 активирован (период 500 мс)");
    } 
    else if (command == '2') {
      currentMode = MODE_2;
      previousMillis = myMillis();
      Serial.println("✓ РЕЖИМ 2 активирован (период 1000 мс)");
    }
  }
  
  unsigned long currentMillis = myMillis();
  
  // Режим 1 - период 500 мс
  if (currentMode == MODE_1) {
    if (currentMillis - previousMillis >= 500) {
      previousMillis = currentMillis;
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
  }
  
  // Режим 2 - период 1000 мс
  if (currentMode == MODE_2) {
    if (currentMillis - previousMillis >= 1000) {
      previousMillis = currentMillis;
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
  }
}

void setupTimer2_Interrupt() {
  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCNT2 = 0;
  TCCR2A |= (1 << WGM21);
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
  OCR2A = 255;
  TIMSK2 |= (1 << OCIE2A);
  sei();
}

void setupTimer1_CustomMillis() {
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS11) | (1 << CS10);
  OCR1A = 249;
  TIMSK1 |= (1 << OCIE1A);
  sei();
}

ISR(TIMER2_COMPA_vect) {
  timer2_counter++;
  if (timer2_counter >= 122) {
    timer2_counter = 0;
    PORTD ^= (1 << 2); 
  }
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