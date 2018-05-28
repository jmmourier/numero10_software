// todo : create class for H bridge
// todo : create class for encoder
// todo : create class for PID 

// pin 
int motor1Fw = 10;
int motor1Bw = 11;
int motor1EnA = 3;
int motor1EnB = 5;

int motor2Fw = 6;
int motor2Bw = 9;
int motor2EnA = 2;
int motor2EnB = 4;


// variables 
bool doAsserv;
// Motor1
long motor1Encoder = 0;
long motor1EncoderOld = 0;
long targetMotor1;
// Motor2
long motor2Encoder = 0;
long motor2EncoderOld = 0;
long targetMotor2;

int memoSpeed;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // motor as output
  pinMode(motor1Fw, OUTPUT);
  pinMode(motor1Bw, OUTPUT);
  pinMode(motor2Fw, OUTPUT);
  pinMode(motor2Bw, OUTPUT);

  // set motor as 0
  analogWrite(motor1Fw, 0);
  analogWrite(motor1Bw, 0);
  analogWrite(motor2Fw, 0);
  analogWrite(motor2Bw, 0);

  // coder as input
  pinMode(motor1EnA, INPUT_PULLUP);
  pinMode(motor1EnB, INPUT);
  pinMode(motor2EnA, INPUT_PULLUP);
  pinMode(motor2EnB, INPUT);

  attachInterrupt(digitalPinToInterrupt(motor1EnA), handleEncoderMotor1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(motor2EnA), handleEncoderMotor2, CHANGE);

  // init variables
  doAsserv = false;
  targetMotor1 = 0;
  targetMotor2 = 0;
  memoSpeed = 0;

  // mode de test de l'asserv
  doAsserv = true;
  setTarget(1,0);
  setTarget(2,0);
}


void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    char order = Serial.read();
    switch(order) 
    {
      case 'a':
      setMotorSpeed(1,80);
      setMotorSpeed(2,80);
      /*    
      analogWrite(motor1Fw, 80);
      analogWrite(motor1Bw, 0);
      analogWrite(motor2Fw, 80);
      analogWrite(motor2Bw, 0); 
      */
      break;
      
      case 'b':
      setMotorSpeed(1,-80);
      setMotorSpeed(2,-80);
      /*
      analogWrite(motor1Fw, 0);
      analogWrite(motor1Bw, 80);
      analogWrite(motor2Fw, 0);
      analogWrite(motor2Bw, 80);
      */
      break;
      
      case 'c':
      setMotorSpeed(1,0);
      setMotorSpeed(2,0);
      /*
      analogWrite(motor1Fw, 0);
      analogWrite(motor1Bw, 0);
      analogWrite(motor2Fw, 0);
      analogWrite(motor2Bw, 0);
      */
      break;

      case 'd':
      doAsserv = true;
      setTarget(1,244);
      setTarget(2,-244);
      break;
      
      case 'e':
      doAsserv = true;
      setTarget(1,0);
      setTarget(2,0);
      break;
      
      case 'f':
      doAsserv = false;
      break;
      
      case 'q':
      doAsserv = false;
      memoSpeed ++;
      if(memoSpeed > 255) memoSpeed = 255;
      setMotorSpeed(1,memoSpeed);
      setMotorSpeed(2,memoSpeed);
      Serial.print("order : ");
      Serial.println(memoSpeed);
      break;
      
      case 's':
      doAsserv = false;
      memoSpeed --;
      if(memoSpeed < 0)memoSpeed = 0; 
      setMotorSpeed(1,memoSpeed);
      setMotorSpeed(2,memoSpeed);
      Serial.print("order : ");
      Serial.println(memoSpeed);
      break;
      
      default:
      
      break;
    }
  }

  if(doAsserv)
    handleAsserv();

/*
   Serial.print(motor1Encoder);
   Serial.print(" ");
   Serial.println(motor2Encoder);
*/
}

void setTarget(char idMotor, signed long Target)
{
  if(idMotor == 1)
    targetMotor1 = Target;
  else
    targetMotor2 = Target;
}

long sumErrorMotor1 = 0;
long sumErrorMotor2 = 0;
unsigned long deltaTAsserv = 0; 
unsigned long lastAsserv = 0; 
void  handleAsserv()
{
  // asserv Motor1
  double Kp = 0.7;
  double Ki = 0.0001;
  double Kd = 0.01;
  deltaTAsserv = millis() - lastAsserv; 
  

  long errorMotor1 = targetMotor1 - motor1Encoder;
  sumErrorMotor1 += errorMotor1;
  long derivErrorMotor1 = errorMotor1 / deltaTAsserv;  

  int propMotor1 = Kp*errorMotor1;
  int derivMotor1 = 0;
  int integMotor1 = 0;
  int limitIntegMotor1 = 10;
  if(integMotor1>limitIntegMotor1) integMotor1 = limitIntegMotor1;
  if(integMotor1<-limitIntegMotor1) integMotor1 = -limitIntegMotor1;

  int commandMotor1 = propMotor1 + integMotor1 + derivMotor1;
  if(commandMotor1 >150) commandMotor1 = 150;
  if(commandMotor1 <-150) commandMotor1 = -150;
  if(commandMotor1 >-25 && commandMotor1 <25) commandMotor1 = 0;
  
  setMotorSpeed(1,commandMotor1);

  // asserv Motor2
  Serial.print("target is ");
  Serial.print(targetMotor1);
  Serial.print("   error is ");
  Serial.print(errorMotor1);
  Serial.print("   counter is ");
  Serial.print(motor1Encoder);
  Serial.print("  output is ");
  Serial.print(commandMotor1);
  Serial.print("  prop is ");
  Serial.print(propMotor1);


  
  long errorMotor2 = targetMotor2 - motor2Encoder;
  sumErrorMotor2 += errorMotor2;
  long derivErrorMotor2 = errorMotor2 / deltaTAsserv;  

  int propMotor2 = Kp*errorMotor2;
  int derivMotor2 = 0;
  int integMotor2 = 0;
  int limitIntegMotor2 = 10;
  if(integMotor2>limitIntegMotor2) integMotor2 = limitIntegMotor2;
  if(integMotor2<-limitIntegMotor2) integMotor2 = -limitIntegMotor2;

  int commandMotor2 = propMotor2 + integMotor2 + derivMotor2;
  if(commandMotor2 >150) commandMotor2 = 150;
  if(commandMotor2 <-150) commandMotor2 = -150;
  if(commandMotor2 >-25 && commandMotor2 <25) commandMotor2 = 0;

  setMotorSpeed(2,commandMotor2);

  // asserv Motor2
  Serial.print("   target is ");
  Serial.print(targetMotor2);
  Serial.print("   error is ");
  Serial.print(errorMotor2);
  Serial.print("   counter is ");
  Serial.print(motor2Encoder);
  Serial.print("  output is ");
  Serial.print(commandMotor2);
  Serial.print("  prop is ");
  Serial.print(propMotor2);

  Serial.println("");
  
  lastAsserv = millis();
}

void setMotorSpeed(char idMotor, signed int speed)
{
  if(speed > 255) speed = 255;
  if(speed < -255) speed = -255;
  if(speed > 0)
  {
    if(idMotor == 1)
    {
      analogWrite(motor1Fw,speed);
      analogWrite(motor1Bw,0);
    }
    else
    {
      analogWrite(motor2Fw,speed);
      analogWrite(motor2Bw,0);
    }
  }
  else
  {
    if(idMotor == 1)
    {
      analogWrite(motor1Fw,0);
      analogWrite(motor1Bw,-speed);
    }
    else
    {
      analogWrite(motor2Fw,0);
      analogWrite(motor2Bw,-speed);
    }
  }
}


void handleEncoderMotor1()
{
  //Serial.print("interupt1 ");
  int A = digitalRead(motor1EnA);
  int B = digitalRead(motor1EnB);
  //Serial.print("A ");
  //Serial.print(A);
  //Serial.print(" B ");
  //Serial.println(B);
  if(A == 1)
  {
   if(B == 0)
    motor1Encoder++; 
   else
    motor1Encoder--;
  }
  else // if(A == 0)
  {
   if(B == 1)
    motor1Encoder++; 
   else
    motor1Encoder--;
  }
  motor1EncoderOld = motor1Encoder;
}

void handleEncoderMotor2()
{
  //Serial.println("interupt2");
  int A = digitalRead(motor2EnA);
  int B = digitalRead(motor2EnB);
  if(A == 1)
  {
   if(B == 0)
    motor2Encoder--; 
   else
    motor2Encoder++;
  }
  else // if(A == 0)
  {
   if(B == 1)
    motor2Encoder--; 
   else
    motor2Encoder++;
  }
  motor2EncoderOld = motor2Encoder;
}


