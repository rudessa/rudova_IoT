const int PHOTORESISTOR_PIN = A1;
const int DELAY_MS = 1000;

bool streamMode = false;

void setup()
{
  Serial.begin(9600);
  pinMode(PHOTORESISTOR_PIN, INPUT);

  while (!Serial)
  {
    ;
  }
}

void loop()
{
  if (Serial.available() > 0)
  {
    char cmd = Serial.read();

    while (Serial.available() > 0)
    {
      Serial.read();
    }

    if (cmd == 'p')
    {
      int sensorValue = analogRead(PHOTORESISTOR_PIN);
      Serial.print("SENSOR_VALUE:");
      Serial.println(sensorValue);
    }
    else if (cmd == 's')
    {
      streamMode = true;
      Serial.println("STREAM_STARTED");
    }
    else if (cmd == 'x')
    {
      streamMode = false;
      Serial.println("STREAM_STOPPED");
    }
  }

  if (streamMode)
  {
    int sensorValue = analogRead(PHOTORESISTOR_PIN);
    Serial.print("SENSOR_VALUE:");
    Serial.println(sensorValue);
    delay(DELAY_MS);
  }
}