#define DIR_LEFT 4
#define SPEED_LEFT 5

#define DIR_RIGHT 7
#define SPEED_RIGHT 6

void setup(){
  pinMode(DIR_LEFT, OUTPUT);
  pinMode(DIR_RIGHT, OUTPUT);
  pinMode(SPEED_LEFT, OUTPUT);
  pinMode(SPEED_RIGHT, OUTPUT);

  digitalWrite(DIR_LEFT, LOW);
  digitalWrite(DIR_RIGHT, HIGH);
  digitalWrite(SPEED_LEFT, LOW);
  digitalWrite(SPEED_RIGHT, HIGH);

  delay(2000);

  digitalWrite(DIR_LEFT, low);
  digitalWrite(DIR_RIGHT, LOW);
  digitalWrite(SPEED_LEFT, HIGH);
  digitalWrite(SPEED_RIGHT, LOW);

}


void loop(){
  digitalWrite(DIR_LEFT, LOW);
  digitalWrite(DIR_RIGHT, HIGH);
  digitalWrite(SPEED_LEFT, LOW);
  digitalWrite(SPEED_RIGHT, HIGH);
}

