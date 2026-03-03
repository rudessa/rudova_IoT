#define DIR_LEFT 4
#define SPEED_LEFT 5

#define DIR_RIGHT 7
#define SPEED_RIGHT 6

#define FORWARD_LEFT LOW
#define BACKWARD_LEFT HIGH
#define FORWARD_RIGHT HIGH
#define BACKWARD_LEFT LOW



//Функция универсальная 

void move(bool left_dir, int left_speed, bool right_dir, int right_speed) {
  digitalWrite(DIR_LEFT, left_dir);
  digitalWrite(DIR_RIGHT, right_dir);
  analogWrite(SPEED_LEFT, left_speed);
  analogWrite(SPEED_RIGHT, right_speed);

}


void setup(){
  pinMode(DIR_LEFT, OUTPUT);
  pinMode(DIR_RIGHT, OUTPUT);
  pinMode(SPEED_LEFT, OUTPUT);
  pinMode(SPEED_RIGHT, OUTPUT);
  move(FORWARD_LEFT, 255, FORWARD_RIGHT, 0 );
  delay(2000);
  move(FORWARD_LEFT, 0, FORWARD_RIGHT, 0);
}


void loop(){
  digitalWrite(DIR_LEFT, LOW);
  digitalWrite(DIR_RIGHT, HIGH);
  digitalWrite(SPEED_LEFT, LOW);
  digitalWrite(SPEED_RIGHT, HIGH);
}

