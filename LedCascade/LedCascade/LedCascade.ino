#define F_CPU 16000000UL

#define SERIAL_BAUD 115200
#define PLOT_ENABLED true 

#define LED1_PIN PB0
#define LED2_PIN PB1
#define LED3_PIN PB2
#define LED4_PIN PB3
#define LED5_PIN PB4

#define TIMER1_PRESCALER 256
#define BASE_INTERVAL_MS 100  

#define OCR1A_VALUE 6249
#define LED1_PERIOD 1 
#define LED2_PERIOD 2  
#define LED3_PERIOD 3  
#define LED4_PERIOD 4  
#define LED5_PERIOD 5  

volatile uint8_t counter1 = 0;
volatile uint8_t counter2 = 0;
volatile uint8_t counter3 = 0;
volatile uint8_t counter4 = 0;
volatile uint8_t counter5 = 0;

volatile bool send_plot_data = false;

void timer1_init() {
  cli();
  
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  
  TCCR1B |= (1 << WGM12);
  
  TCCR1B |= (1 << CS12);
  
  OCR1A = OCR1A_VALUE;
  
  TIMSK1 |= (1 << OCIE1A);
  
  sei();
}

void ports_init() {
  DDRB |= (1 << LED1_PIN) | (1 << LED2_PIN) | (1 << LED3_PIN) | 
          (1 << LED4_PIN) | (1 << LED5_PIN);
  
  PORTB |= (1 << LED1_PIN) | (1 << LED2_PIN) | (1 << LED3_PIN) | 
           (1 << LED4_PIN) | (1 << LED5_PIN);
}

ISR(TIMER1_COMPA_vect) {
  counter1++;
  counter2++;
  counter3++;
  counter4++;
  counter5++;
  
  if (counter1 >= LED1_PERIOD) {
    PORTB ^= (1 << LED1_PIN);  
    counter1 = 0;
  }
  
  if (counter2 >= LED2_PERIOD) {
    PORTB ^= (1 << LED2_PIN);
    counter2 = 0;
  }
  
  if (counter3 >= LED3_PERIOD) {
    PORTB ^= (1 << LED3_PIN);
    counter3 = 0;
  }
  
  if (counter4 >= LED4_PERIOD) {
    PORTB ^= (1 << LED4_PIN);
    counter4 = 0;
  }
  
  if (counter5 >= LED5_PERIOD) {
    PORTB ^= (1 << LED5_PIN);
    counter5 = 0;
  }
  send_plot_data = true;
}

void setup() {
  if (PLOT_ENABLED) {
    Serial.begin(SERIAL_BAUD);
    while (!Serial) {
      ; 
    }
  }
  ports_init();
  timer1_init();
}

void loop() {
  if (PLOT_ENABLED && send_plot_data) {
    send_plot_data = false;

    uint8_t led1_state = (PORTB & (1 << LED1_PIN)) ? 1 : 0;
    uint8_t led2_state = (PORTB & (1 << LED2_PIN)) ? 2 : 0;
    uint8_t led3_state = (PORTB & (1 << LED3_PIN)) ? 3 : 0;
    uint8_t led4_state = (PORTB & (1 << LED4_PIN)) ? 4 : 0;
    uint8_t led5_state = (PORTB & (1 << LED5_PIN)) ? 5 : 0;
    
    Serial.print("LED1:");
    Serial.print(led1_state);
    Serial.print(",LED2:");
    Serial.print(led2_state);
    Serial.print(",LED3:");
    Serial.print(led3_state);
    Serial.print(",LED4:");
    Serial.print(led4_state);
    Serial.print(",LED5:");
    Serial.println(led5_state);
  }
}