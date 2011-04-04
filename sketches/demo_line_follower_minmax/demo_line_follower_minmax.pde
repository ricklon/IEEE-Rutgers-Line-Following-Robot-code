/*
* Demo Line follower code for the IEEE Rutgers Line Following robot
* Some quick clean up done 20110403
*
*/
#include <avr/interrupt.h>

#define motor1Dir 7
#define motor2Dir 8
#define motor1PWM 9
#define motor2PWM 10
#define motor1Enable 11
#define motor2Enable 12
#define numSensors 5
#define line_thresh 100

//Decalre sensors
int sensors[numSensors];
int sensorsmax[numSensors] =
{
  -1,-1,-1,-1,-1
};
int sensorsmin[numSensors] = 
{
  1024,1024,1024,1024,1024
};


void setMotorVel(int dirPin, int pwmPin, int velocity){
  if (velocity >= 255) velocity = 255;
  if (velocity <= -255) velocity = -255;

  if (velocity == 0)
  {
    digitalWrite(dirPin, HIGH);
    digitalWrite(pwmPin, HIGH);
  }
  else if(velocity <0){  // Reverse
    digitalWrite(dirPin, HIGH);
    analogWrite(pwmPin, 255+velocity);
  }
  else if(velocity >0){ // Forward
    digitalWrite(dirPin,LOW);
    analogWrite(pwmPin, velocity);
  }
}


void setLeftMotorSpeed(int velocity)
{
  setMotorVel(motor1Dir, motor1PWM, -velocity);

}

void setRightMotorSpeed(int velocity){
  setMotorVel(motor2Dir, motor2PWM, -velocity);
}

void initMotorDriver()
{
  pinMode(motor1Dir, OUTPUT);
  pinMode(motor2Dir, OUTPUT);

  pinMode(motor1Enable, OUTPUT);
  pinMode(motor2Enable, OUTPUT);
  digitalWrite(motor1Enable,HIGH);
  digitalWrite(motor2Enable,HIGH);
  setLeftMotorSpeed(0); // make sure the motors are stopped
  setRightMotorSpeed(0);
}




void goForward()
{
  setLeftMotorSpeed(255);
  setRightMotorSpeed(255);
}

void goRight()
{
  setLeftMotorSpeed(255);
  setRightMotorSpeed(-255);
}

void goLeft()
{
  setLeftMotorSpeed(-255);
  setRightMotorSpeed(255);
}


void setup(){
  Serial.begin(115200); //Set the buad rate for the serial com. 

  // prints title with ending line break 
  Serial.println("Line Sensor board Sensor test"); 
  initMotorDriver();
  
  
}


void loop(){

 for (int ii =0; ii <numSensors; ii++)
  {
    sensors[ii] = analogRead(ii);
    sensorsmin[ii] = min(sensorsmin[ii], sensors[ii]);
    sensorsmax[ii] = max(sensorsmax[ii], sensors[ii]);
  }

  Serial.print("Sensor Values: " );

  for (int ii =0; ii <numSensors; ii++)
  {
    Serial.print("s");
    Serial.print(ii);
    Serial.print("= " );
    Serial.print( sensors[ii]);
    Serial.print(" Max: " );
    Serial.print( sensorsmax[ii]);
    Serial.print(" Min: " );   
    Serial.print( sensorsmin[ii]); 
     Serial.print(", " );   
  }

  Serial.println(" ");

  if(( sensors[3] < line_thresh) || (sensors[4] <line_thresh))  // 250 is a measured threshold between white & black
  {
    Serial.println("goLeft.");
    goLeft(); 
    delay(30);
  }
   else if(( sensors[0] <line_thresh) || (sensors[1] <line_thresh))
  {
    Serial.println("goRight.");
    goRight();
    delay(30);
  }
  else
  {
   Serial.println("goForward.");
   goForward();
   delay(100);
  }
   
}
