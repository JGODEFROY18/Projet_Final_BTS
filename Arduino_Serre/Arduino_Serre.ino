//Arduino PWM Speed Control：
const int E1 = 5;
const int M1 = 4;
const int E2 = 6;
const int M2 = 7;
const int carte = 2;
int val = 0;

void setup()
{
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
}

void loop()
{
    val = digitalRead(carte);
    if (val == 1)
    {
      Serial.println ("Ouverture Fenêtre");
      digitalWrite(M1, HIGH);  
      analogWrite(E1, 240);   //PWM Speed Control 
    }
    else
    {
      Serial.println("Fermeture Fenêtre");
      digitalWrite(M1, LOW);  
      analogWrite(E1, 240);   //PWM Speed Control 
    }
}
