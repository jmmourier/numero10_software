int moteur1 = 10;
int moteur2 = 11;

unsigned char moteur1Value = 127;
unsigned char moteur2Value = 127;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(moteur1, OUTPUT);
  pinMode(moteur2, OUTPUT);
  
      analogWrite(moteur1, moteur1Value);
      analogWrite(moteur2, moteur2Value);
}


void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    char order = Serial.read();
    switch(order) 
    {
      case 'a':
      moteur1Value = 127;
      break;
      
      case 'b':
      moteur1Value = 255;
      break;
      
      case 'c':
      moteur2Value = 127;
      break;
      
      case 'd':
      moteur2Value = 255;
      break;
      
      case '+':
      moteur1Value++;
      moteur2Value++;
      break;
      
      case '-':
      moteur1Value--;
      moteur2Value--;
      break;
      
      default:
      Serial.println("XXX recu");
      break;
    }
    
      Serial.print("moteur 1 : ");
      Serial.print(moteur1Value);
      Serial.print("   moteur 2 : ");
      Serial.println(moteur2Value);

      analogWrite(moteur1, moteur1Value);
      analogWrite(moteur2, moteur2Value);
  }
}

