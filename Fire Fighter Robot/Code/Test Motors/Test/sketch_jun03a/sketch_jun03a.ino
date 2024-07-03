//Robot 

//libs part 
#include<Servo.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>

/************/
//Define All pins 
#define Room1Event   1
#define Room2Event   4
#define ServoPin   5
#define TrigPin    6
#define EchoPin    7
#define BlueTx     8
#define BlueRx     9
#define GSMTx      10
#define GSMRx      11  
#define FlamPin    A0
#define PumbPin    A1
#define PumbPWM    3
#define Motor1A    A2
#define Motor1B    A3
#define Motor2A    A4
#define Motor2B    A5
#define IRSensor1  12
#define IRSensor2  13

#define SafeZone   20 
#define Rotate_Time 500
/***********/

//defines Global Variables
Servo Myservo;
SoftwareSerial GSMSerial(GSMRx, GSMTx); // RX, TX pins in arduino 
SoftwareSerial BluetoothSerial(BlueRx, BlueTx); // RX, TX pins in arduino 
int Servopos = 0;
const byte BlueOrder = 0;
bool ThereFire1 = false ;
bool ThereFire2 = false ;
long Duration;
int Distance;
const float SoundSpeed = 0.034 ;
bool StopPostion = false ;
bool FirePostionshere = false ;
int NowPostions ;
int RoomCounter = 0 ;
bool GetOut = false ;//for get out from room after fire off 
int Manual = 0 ; 
int EEPROMAddress = 0 ;
char IncomingData = 0; 
int GetOutCounter = 0 ;
int ReadBlue = LOW;
String PHONE_NUMBER = "01119897972";//put your phone here as a string 

/**********/

//Declare functions 
void BluetoothOrderHandle();
void Forword();
void Left();
void Right();
void Rotate_Right();
void Rotate_Left();
void Back();
void Stop();
void Break();
void FoundObstacle(); //Edit 
void FireFighting();//Edit 
void UltrON();
void SendMessage(int RoomNumberMsg);
float VoltRead(float AnalogRead);

/******************/
//Setup and init. all my H.W
void setup() 
{
Myservo.attach(ServoPin); // to init my servo motor
Myservo.write(Servopos); // start postion at 0 angle 
pinMode(TrigPin, OUTPUT); // Sets the TrigPin as an Output
pinMode(EchoPin, INPUT); // Sets the EchoPin as an Input
pinMode(FlamPin, INPUT);
pinMode(PumbPin, OUTPUT);//connected with transistor 
pinMode(Motor1A, OUTPUT);
pinMode(Motor1B, OUTPUT);
pinMode(Motor2A, OUTPUT);
pinMode(Motor2B, OUTPUT);
pinMode(IRSensor1, INPUT);//for line follower 
pinMode(IRSensor2, INPUT);
pinMode(Room1Event, INPUT);
pinMode(Room2Event, INPUT);
pinMode(Room2Event, INPUT);
pinMode(BlueOrder, INPUT);
GSMSerial.begin(9600);
BluetoothSerial.begin(9600);
//Stop();
Myservo.write(90);
delay(2000);

  
}

void loop() 
{
/*
            if(digitalRead(IRSensor1)==HIGH && digitalRead(IRSensor2)==HIGH) // here all the reads are black so this stop station of the robot // End Line 
            {
                Stop();   
            }
            else if (digitalRead(IRSensor1)==HIGH && digitalRead(IRSensor2)==LOW) //the black line on the left so we need to correct out road to the right
            {
               Right(); 
            }
            else if (digitalRead(IRSensor1)==LOW && digitalRead(IRSensor2)==HIGH) // the black line on the right so we need to correct out road to the left
            {
               Left(); 
            }
            else if (digitalRead(IRSensor1)==LOW && digitalRead(IRSensor2)==LOW) // no Black Line So keep move forword
            {
              Forword(); 
            }
  */
  Forword();
   delay(3000);
  Left();
  delay(3000);
  Right();
  delay(3000);
    Back();
  delay(3000);
  Rotate_Right();
    delay(3000); 
    Rotate_Left();
     delay(3000);
}


//Notes - TO DO :

//GSM code 




//Functions Part 
void Forword()
{
    digitalWrite(Motor1A, LOW); 
  digitalWrite(Motor1B, HIGH); 
  digitalWrite(Motor2A, LOW); 
  digitalWrite(Motor2B, HIGH);
}

void Left()
{
  digitalWrite(Motor1A, LOW); 
  digitalWrite(Motor1B, HIGH); 
  digitalWrite(Motor2A, LOW); 
  digitalWrite(Motor2B, LOW);
}

void Right()
{
    digitalWrite(Motor1A, LOW); 
  digitalWrite(Motor1B, LOW); 
  digitalWrite(Motor2A, LOW); 
  digitalWrite(Motor2B, HIGH); 

  
}

void Rotate_Right()
{
    digitalWrite(Motor1A, HIGH); 
  digitalWrite(Motor1B, LOW); 
  digitalWrite(Motor2A, LOW); 
  digitalWrite(Motor2B, HIGH); 

}

void Rotate_Left()
{
  digitalWrite(Motor1A, LOW); 
  digitalWrite(Motor1B, HIGH); 
  digitalWrite(Motor2A, HIGH); 
  digitalWrite(Motor2B, LOW); 
}

void Back()
{
    digitalWrite(Motor1A, HIGH); 
  digitalWrite(Motor1B, LOW); 
  digitalWrite(Motor2A, HIGH); 
  digitalWrite(Motor2B, LOW);
  
}

void Break()//make all motors off 
{
  digitalWrite(Motor1A, HIGH); 
  digitalWrite(Motor1B, HIGH); 
  digitalWrite(Motor2A, HIGH); 
  digitalWrite(Motor2B, HIGH);  
}

void Stop()//make all motors off 
{
  digitalWrite(Motor1A, LOW); 
  digitalWrite(Motor1B, LOW); 
  digitalWrite(Motor2A, LOW); 
  digitalWrite(Motor2B, LOW);  
}


float VoltRead(float AnalogRead)
{
  float Voltage = (AnalogRead / 1024) * 5 ;
  return Voltage ;
}

void UltrON()
{
    digitalWrite(TrigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(TrigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(TrigPin, LOW);
    Duration = pulseIn(EchoPin, HIGH);
    Distance = (Duration*SoundSpeed)/2;
}

//repet the design of the fun 






            /* for(  ;  Servopos <= (NowPostions + 20 ) ; Servopos++)
              {
                 
                  Myservo.write(Servopos);
                  delay(50);
              }
              NowPostions = Servopos ;
               for(       ;   Servopos >= (NowPostions - 40 );Servopos--)
              {
                if(Servopos == 0)//to save the servo gears 
                {
                  break;
                }
                  digitalWrite(PumbPin , HIGH);
                  Myservo.write(Servopos);
                  delay(50);
              }
              NowPostions = Servopos + 20 ;*/

