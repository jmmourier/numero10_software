void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);

  analogWrite(10,0);
  analogWrite(11,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(10,0);
  analogWrite(11,80);
  
  analogWrite(9,0);
  analogWrite(6,80);
  delay(1000);
  
  analogWrite(10,255);
  analogWrite(11,255);
  
  analogWrite(9,255);
  analogWrite(6,255);
  delay(1000);
  
  analogWrite(10,80);
  analogWrite(11,0);
  
  analogWrite(9,80);
  analogWrite(6,0);
  delay(1000);
}
