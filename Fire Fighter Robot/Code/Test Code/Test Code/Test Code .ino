//libs part 
#include<Servo.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>

/************/
//Define All pins 
#define Room1Event   3
#define Room2Event   4
#define ServoPin   5
#define TrigPin    8
#define EchoPin    7
#define GSMTx      10
#define GSMRx      11  
#define FlamPin    A0
#define PumbPin    A1
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
SoftwareSerial MySerial(GSMRx, GSMTx); // RX, TX pins in arduino 
int Servopos = 0;
const byte BlueOrder = 2;
bool ThereFire1 = false ;
bool ThereFire2 = false ;
long Duration;
int Distance;
const float SoundSpeed = 0.034 ;
bool StopPostion = false ;
bool FirePostionshere = false ;
int NowPostions ;
int RoomCounter = 0 ;
bool GetOut = false ;
int Manual = 0 ; 
int EEPROMAddress = 0 ;
char IncomingData = 0;   
bool COUNTERT = true ;
/**********/

//Declare functions 
void BluetoothOrderHandle();
void Forword();
void Left();
void Right();
void Rotate();
void Stop();
void Break();
void FoundObstacle(); //Edit 
void FireFighting();//Edit 
void UltrON();

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
pinMode(Motor2B, OUTPUT);
pinMode(Motor2A, OUTPUT);
pinMode(Motor2B, OUTPUT);
pinMode(IRSensor1, INPUT);//for line follower 
pinMode(IRSensor2, INPUT);
pinMode(Room1Event, INPUT);
pinMode(Room2Event, INPUT);
pinMode(BlueOrder, INPUT_PULLUP);
//Manual = EEPROM.read(EEPROMAddress);
Manual = 1 ;
//attachInterrupt(digitalPinToInterrupt(BlueOrder), BluetoothOrderHandle , CHANGE);
Serial.begin(9600);
MySerial.begin(9600);
Serial.println("Welcome");
Serial.println("Robot In ON Mode");
Stop();
Myservo.write(90);
delay(200);
pinMode(6 , OUTPUT);
pinMode(3 , OUTPUT);
analogWrite(6 , 80);
analogWrite(3 , 80);
}

void loop() 
{//Start Code 

 /*********************************************/ // Bluetooth Control 

   /*  while(Manual == 1) 
     {
       if(Serial.available() > 0)  
     { 
        IncomingData = Serial.read();      //Read the incoming data and store it into variable Incoming_value
        if(IncomingData == 'F')            //Checks whether value of IncomingData is equal to 1 
        {
          Forword();
        }
        else if (IncomingData == 'L')
        {
          Left();
        }
        else if (IncomingData == 'R')
        {
          Right();
        }
        else if (IncomingData == 'W')
        {
          FireFighting();
        }
        else 
        {
          Stop();
        }
        Serial.println(IncomingData);
       // delay(300);
        
      }  
     }
*/



while(digitalRead(FlamPin) == LOW && COUNTERT )
{
  
for(int count= 0 ; count <=180 ; count++)
{
  if(digitalRead(FlamPin) == HIGH)
  {
      COUNTERT = false;
      break;
  
  }

  Myservo.write(count); 
 delay(10);
}

for(int count= 180 ; count >= 0 ; count--)
{
  
  
  if(digitalRead(FlamPin) == HIGH)
    {
          COUNTERT = false;
      break;

  }
  Myservo.write(count); 
 delay(20);
}

}



UltrON();
  if(digitalRead(IRSensor1)==HIGH && digitalRead(IRSensor2)==HIGH || Distance<10 ) // here all the reads are black so this stop station of the robot // End Line 
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
 
  if(false)
 {
  
 ThereFire1 = digitalRead(Room1Event);
 ThereFire2 = digitalRead(Room2Event);
 
 /*********************************************/ // Room 1  
  if(ThereFire1 == HIGH)
  {
    //Start to Fire 
    Serial.println("There Are a Fire In Room 1"); 
    //message sms
/*******************/   /**************************/
GotoStart1 : 
      Forword(); // make the robot move forword and keep move untill state chnage 
      //turn ultrasonic sensor 
      while(!StopPostion)
      {
              UltrON();
      /**************************************************************************************************************************************************/
             //Obstacle Found Algorithm
            if(Distance < SafeZone)
            {
              Serial.println("There Are Obstacle Here!");
              FoundObstacle();   
            }
            
/**************************************************************************************************************************************************/
           // Normal Move Algorithm
            if(digitalRead(IRSensor1)==HIGH && digitalRead(IRSensor2)==HIGH) // here all the reads are black so this stop station of the robot // End Line 
            {
              RoomCounter++ ;//Every Check Point will be up count here 
              if(GetOut) // the start point in the path after get out from the room and we need here to make the robot stop in the start point 
              {
                GetOut = false ;
                RoomCounter = 0 ;
                break;      
              }
              else if(RoomCounter > 1)// The End point in the room 
              {
                Break();
                StopPostion = true ;
              }
              else // the Road of the First Room 
              {
                Right();
                delay(100);
              }
      
            }
            else if (digitalRead(IRSensor1)==HIGH && digitalRead(IRSensor2)==LOW) //the black line on the left so we need to correct out road to the right
            {
              Right(); 
            }
            else if (digitalRead(IRSensor1)==HIGH && digitalRead(IRSensor2)==LOW) // the black line on the right so we need to correct out road to the left
            {
              Left(); 
            }
            else if (digitalRead(IRSensor1)==LOW && digitalRead(IRSensor2)==LOW) // no Black Line So keep move forword
            {
              Forword(); 
            }
      }
/**************************************************************************************************************************************************/
     //Fire Fighting Algorithm 
      if(GetOut)
     {
          goto GotoStart1;
     }
     else if(StopPostion) // that mean the robot stop here to Search about the fire 
     {
        FireFighting();
     }

/*******************/  /**************************/
    //End Fire  
    Serial.println("Fire 1 OFF");
    //sms fire off
    ThereFire1 = false ;
  }

 
 /*********************************************/ // Room 2 
  if(ThereFire1 == HIGH)
  {
    //Start to Fire 
    Serial.println("There Are a Fire In Room 1"); 
    //message sms
/*******************/   /**************************/
GotoStart : 
      Forword(); // make the robot move forword and keep move untill state chnage 
      //turn ultrasonic sensor 
      while(!StopPostion)
      {
              UltrON();
      /**************************************************************************************************************************************************/
             //Obstacle Found Algorithm
            if(Distance < SafeZone)
            {
              Serial.println("There Are Obstacle Here!");
              FoundObstacle();   
            }
            
/**************************************************************************************************************************************************/
           // Normal Move Algorithm
            if(digitalRead(IRSensor1)==HIGH && digitalRead(IRSensor2)==HIGH) // here all the reads are black so this stop station of the robot // End Line 
            {
              RoomCounter++ ;//Every Check Point will be up count here 
              if(GetOut) // the start point in the path after get out from the room and we need here to make the robot stop in the start point 
              {
                //don't out from here untill arrive to the start point 
                //
                GetOut = false ;
                RoomCounter = 0 ;
                break;      
              }
              else if(RoomCounter > 1)// The End point in the room 
              {
                Break();
                StopPostion = true ;
              }
              else // the Road of the First Room 
              {
                Forword();
                delay(100);
              }
      
            }
            else if (digitalRead(IRSensor1)==HIGH && digitalRead(IRSensor2)==LOW) //the black line on the left so we need to correct out road to the right
            {
              Right(); 
            }
            else if (digitalRead(IRSensor1)==HIGH && digitalRead(IRSensor2)==LOW) // the black line on the right so we need to correct out road to the left
            {
              Left(); 
            }
            else if (digitalRead(IRSensor1)==LOW && digitalRead(IRSensor2)==LOW) // no Black Line So keep move forword
            {
              Forword(); 
            }
      }
/**************************************************************************************************************************************************/
     //Fire Fighting Algorithm 
      if(GetOut)
     {
          goto GotoStart;
     }
     else if(StopPostion) // that mean the robot stop here to Search about the fire 
     {
        FireFighting();
     }

/*******************/  /**************************/
    //End Fire  
    Serial.println("Fire 1 OFF");
    //sms fire off
    ThereFire1 = false ;
  }


 }

 

}//End Code 

//Notes - TO DO :
//obstacle 
//GSM code 
//check again after fire fighting 
//



//Interrupt Part 
void BluetoothOrderHandle()
{
  Serial.println("Order Received");
  if(Manual == 1)
  {
    Manual = 0 ;
    EEPROM.write(EEPROMAddress , Manual);
  }
  else 
  {
    Manual = 1 ;
    EEPROM.write(EEPROMAddress , Manual);
  }
  
  //Restart the arduino here 
  wdt_disable();
  wdt_enable(WDTO_15MS);
  while(1);
}


//Functions Part 
void Forword()
{
  digitalWrite(Motor1A, HIGH); 
  digitalWrite(Motor1B, LOW); 
  digitalWrite(Motor2A, HIGH); 
  digitalWrite(Motor2B, LOW); 
}

void Left()
{
  digitalWrite(Motor1A, LOW); 
  digitalWrite(Motor1B, LOW); 
  digitalWrite(Motor2A, HIGH); 
  digitalWrite(Motor2B, LOW); 
}

void Right()
{
  digitalWrite(Motor1A, HIGH); 
  digitalWrite(Motor1B, LOW); 
  digitalWrite(Motor2A, LOW); 
  digitalWrite(Motor2B, LOW);  
}

void Rotate()
{
  digitalWrite(Motor1A, LOW); 
  digitalWrite(Motor1B, HIGH); 
  digitalWrite(Motor2A, HIGH); 
  digitalWrite(Motor2B, LOW); 
  delay(Rotate_Time);
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

void FoundObstacle()
{
        Right();
        delay(500);
        Forword();
        Myservo.write(0);
        while(Distance < SafeZone);
        delay(100);// to make sire all the roboto body gte out from the obstacle
        Left();
        delay(300);
        Forword();       
        while(Distance < SafeZone);//wait until pass this obstacle
        Left();
        delay(300);
        Myservo.write(90);
        Forword();
        while(digitalRead(IRSensor1)==LOW);
        delay(50);    
}







void FireFighting()
{
      Serial.println("Robot Stop");
      //sms message 
      Myservo.write(0);//make the start point of the servo at point 0 
      while(!FirePostionshere) //don't get out from here until found the fire position or search for 3 items about the fire and don't found any thing
      {
      // start to search and fight the fire here
        for(Servopos=0;Servopos>=180;Servopos++)
        {

          
           if(digitalRead(FlamPin) == HIGH)//if the lame pind read any thing during servo motion stop, get out the loop and open the pumb 
           {
            Serial.println("Found Fire!");
            //sms message 
            FirePostionshere = true ;
            NowPostions = Servopos ;
            break ;//get out 
           }

           
            Myservo.write(Servopos);
            delay(50);
        }
        
        if(!FirePostionshere && RoomCounter<3)
        {
           Myservo.write(0);
           RoomCounter++; 
        }
        else // did't found any fire 
        {
            Myservo.write(0);
            break;
        }
      }
        if(FirePostionshere)// in case found real fire 
        {
          while(digitalRead(FlamPin) == HIGH) // don't get out from here or close the pumb untill the fire off 
          {
              for(       ;  Servopos <= (NowPostions + 20 )   ;   Servopos++)
              {
                  digitalWrite(PumbPin , HIGH);
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
              NowPostions = Servopos + 20 ;
          }
          //check again about the fire 
          FirePostionshere = false ; // Re Init. all the values again 
          digitalWrite(PumbPin , LOW); // cloase the water pumb 
        }
        //Re Init. every thing again 
        Myservo.write(90);
        StopPostion = false ;
        GetOut = true ;// to make the robot stop at the start point after get out the room 
        Rotate();
        delay(200);
       
}

