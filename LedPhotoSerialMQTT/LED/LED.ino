const int LED_PIN = 13;
const int BLINK_INTERVAL = 500;  

bool blinkMode = false;
unsigned long previousMillis = 0;
bool ledState = LOW;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  while (!Serial) {
    ; 
  }
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    
    while (Serial.available() > 0) {
      Serial.read();
    }
    
    if (cmd == 'u') {
      blinkMode = false;
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED_GOES_ON");
    }
    else if (cmd == 'd') {
      blinkMode = false;
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED_GOES_OFF");
    }
    else if (cmd == 'b') {
      blinkMode = true;
      Serial.println("LED_WILL_BLINK");
    }
  }
  
  if (blinkMode) {
    unsigned long currentMillis = millis();
    
    if (currentMillis - previousMillis >= BLINK_INTERVAL) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }
}