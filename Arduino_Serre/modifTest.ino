//Arduino PWM Speed Control：
const int E1 = 5;
const int M1 = 4;
const int E2 = 6;
const int M2 = 7;
int val = 0;
int inputPin = 8;

void setup()
{
    pinMode(inputPin, INPUT_PULLUP);
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
  val = digitalRead(inputPin);
    Serial.println(val);
    if (val == HIGH)
    {
      Serial.println ("Fermeture Fenêtre");
      digitalWrite(M1, LOW);  
      analogWrite(E1, 240);   //PWM Speed Control 
    }
    else
    {
      Serial.println("Ouverture Fenêtre");
      digitalWrite(M1, HIGH);  
      analogWrite(E1,240);   //PWM Speed Control 
    }
}
