//Arduino PWM Speed Control：
const int E1 = 5;
const int M1 = 4;
const int E2 = 6;
const int M2 = 7;
int carte = 0;
int val = 0;
long valsum=0;
int i=1;

void setup()
{
    pinMode(M1, OUTPUT);
    pinMode(M2, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
  i=1;
  valsum=0;
  while(i<=5000){
    val = analogRead(carte);
    valsum=val+valsum;
    delay(1);
    i=i+1;
    //Serial.println(val);
  }
  Serial.println(valsum);
  if(valsum>80){
    val=1;
  }
  else{
    val=0;
    }
    
    if (val < 0)
    {
      Serial.println ("Fermeture Fenêtre");
      digitalWrite(M1, HIGH);  
      analogWrite(E1, 240);   //PWM Speed Control 
    }
    else
    {
      Serial.println("Ouverture Fenêtre");
      digitalWrite(M1, LOW);  
      analogWrite(E1,240);   //PWM Speed Control 
    }
}
