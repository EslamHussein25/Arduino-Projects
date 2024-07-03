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
BluetoothSerial.begin(9600);
Stop();
Myservo.write(90);
delay(200);
}

void loop() 
{
  //Start Code 
 ThereFire1 = digitalRead(Room1Event);
 ThereFire2 = digitalRead(Room2Event);
 ReadBlue = digitalRead(BlueOrder);  
 
 /*********************************************/ // Bluetooth Control 
 if(ReadBlue == HIGH)// Mean Bluetooth have a full control 
 {
     while(ReadBlue == HIGH) 
     {

        IncomingData = BluetoothSerial.read();      //Read the incoming data and store it into variable Incoming_value
        if(IncomingData == 'F')            //Checks whether value of IncomingData is equal to 1 
        {
          Forword();
        }
        else if (IncomingData == 'L')
        {         
           Right();
        }
        else if (IncomingData == 'R')
        {
           Left();
        }
        else if (IncomingData == 'B')
        {
          Back();
        }
        else if (IncomingData == 'U')
        {
          Stop();
        }
            else if (IncomingData == 'G')
        {
         Rotate_Right();
        }
            else if (IncomingData == 'I')
        {
         Rotate_Left();
        }
        else if (IncomingData == 'V')
        {
     
         digitalWrite(PumbPin , HIGH);
        }
             else if (IncomingData == 'v')
        {
         digitalWrite(PumbPin , LOW);
        }
    
 
     }
 }
 

 if(ThereFire1 == HIGH || ThereFire2 == HIGH) // mean will start to move to go to any room in case there are a fire 
 {
  // this var and pins changed by server order to  micrcontroller 
 // make the robot move forword and keep move untill state chnage
 Forword();

 /*********************************************/ // Room 1  
  if(ThereFire1 == HIGH)
  {
    //Start to Fire 
    Serial.println("There Are a Fire In Room 1"); 
/*********************/   /**************************/
GotoStart1 : 
      delay(1000);
      while(!StopPostion) // To Arrive to the end point 
      {
        //tur ultra for all 
              UltrON();
/**************************************************************************************************************************************************/
             //Obstacle Algorithm
            if(Distance < SafeZone)
            {
              Serial.println("There Are Obstacle Here!");
             // FoundObstacle();   
            }
            
/**************************************************************************************************************************************************/
           // Normal Move Algorithm
            if(digitalRead(IRSensor1)==HIGH && digitalRead(IRSensor2)==HIGH) // here all the reads are black so this stop station of the robot // End Line 
            {           
                  RoomCounter++ ;//Every Check Point will be up count here 
                  if(GetOut) //GetOut = false in first time :::  the start point in the path after get out from the room and we need here to make the robot stop in the start point 
                  {
                     if(GetOutCounter == 1) // at the start point we need to stop here and rotate 180 degree 
                     {
                      Rotate_Right();
                        delay(2700);
                        while(!(digitalRead(IRSensor1) == HIGH) && !(digitalRead(IRSensor2) == HIGH));                        
                        delay(300);                        
                        GetOut = false ;
                        RoomCounter = 0 ;
                        GetOutCounter = 0 ;
                        Stop();
                        break;
                      }
                    // to change this var (get out ) state to true must be found the firefighting function start                                   
                    GetOutCounter++;
                    Right();
                    delay(3700);     
                  }                
                  else if(RoomCounter > 1)// The End point in the room 
                  {
                    Break();//stop the robot function 
                    StopPostion = true ;
                  }
                  else // the Road of the First Room 
                  {
                    Left();
                    delay(3700);
                  }     
            }
            else if (digitalRead(IRSensor1)==HIGH && digitalRead(IRSensor2)==LOW) //the black line on the left so we need to correct out road to the right
            {
               
                Left();
            }
            else if (digitalRead(IRSensor1)==LOW && digitalRead(IRSensor2)==HIGH) // the black line on the right so we need to correct out road to the left
            {
                Right();
            }
            else if (digitalRead(IRSensor1)==LOW && digitalRead(IRSensor2)==LOW) // no Black Line So keep move forword
            {
              Forword(); 
            }
      }


/**************************************************************************************************************************************************/
     //Fire Fighting Algorithm 

     if(StopPostion) // that mean the robot stop here to Search about the fire 
     {
        FireFighting(); 
        //the next part to go to the start point
        delay(1000); 
        Rotate_Right();
        delay(2700);//TO MAKE the robot rotate 180 degree 
        while(!(digitalRead(IRSensor1) == HIGH) && !(digitalRead(IRSensor2) == HIGH));   
        delay(300);
        Stop();
        //Re Init. every thing again 
        StopPostion = false ;
        GetOut = true ;// to make the robot stop at the start point after get out the room
        goto GotoStart1;  
     }


/*******************/  /**************************/
    //End Fire  
    Serial.println("Fire 1 OFF");
    ThereFire1 = false ;
 
  }
 
 /*********************************************/ // Room 2 
 /*********************************************/
 /*********************************************/ 

  if(ThereFire2 == HIGH)
  {
    //Start to Fire 
    Serial.println("There Are a Fire In Room 1"); 
/*******************/   /**************************/
GotoStart2 : 

      delay(1000); 
      //turn ultrasonic sensor 
      while(!StopPostion)
      {
              UltrON();
/**************************************************************************************************************************************************/
             //Obstacle Found Algorithm
            if(Distance < SafeZone)
            {
              Serial.println("There Are Obstacle Here!");
             // FoundObstacle();   
            }
            
/**************************************************************************************************************************************************/
           // Normal Move Algorithm
            if(digitalRead(IRSensor1)==HIGH && digitalRead(IRSensor2)==HIGH) // here all the reads are black so this stop station of the robot // End Line 
            {
              RoomCounter++ ;//Every Check Point will be up count here 
              if(GetOut) // the start point in the path after get out from the room and we need here to make the robot stop in the start point 
              {
                  if(GetOutCounter==1)// in start point 
                     {
                     Rotate_Right();
                      delay(5000);
                        while(!(digitalRead(IRSensor1) == HIGH) && !(digitalRead(IRSensor2) == HIGH));
                        
                          delay(1000);
                          Stop();
                        GetOut = false ;
                        RoomCounter = 0 ;
                        GetOutCounter = 0 ;
                        break;
                      }
                    // to change this var (get out ) state to true must be found the firefighting function start                                   
                    GetOutCounter++;
                    Forword();
                    delay(5000);     
              }
              else if(RoomCounter > 1)// The End point in the room 
              {
                Break();
                StopPostion = true ;
              }
              else // the Road of the First Room 
              {
                Forword();
                delay(3000);
              }
      
            }
            else if (digitalRead(IRSensor1)==HIGH && digitalRead(IRSensor2)==LOW) //the black line on the left so we need to correct out road to the right
            {
             
              Left(); 
            }
            else if (digitalRead(IRSensor1)==LOW && digitalRead(IRSensor2)==HIGH) // the black line on the right so we need to correct out road to the left
            {
              Right(); 
            }
            else if (digitalRead(IRSensor1)==LOW && digitalRead(IRSensor2)==LOW) // no Black Line So keep move forword
            {
              Forword(); 
            }
      }
/**************************************************************************************************************************************************/
     //Fire Fighting Algorithm 
      if(StopPostion) // that mean the robot stop here to Search about the fire 
     {
        FireFighting();
        Rotate_Right();
        delay(8000);
        while(!(digitalRead(IRSensor1) == HIGH) && !(digitalRead(IRSensor2) == HIGH));   
        delay(1000);
        Stop();
        goto GotoStart2;
     }

/*******************/  /**************************/
    //End Fire  
    Serial.println("Fire 2 OFF");
    //sms fire off
    ThereFire2 = false ;
  }


 }

 

}//End Code 





//Function part 

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
  digitalWrite(Motor1B, LOW); 
  digitalWrite(Motor2A, LOW); 
  digitalWrite(Motor2B, HIGH);   
}
void Right()
{
  digitalWrite(Motor1A, LOW); 
  digitalWrite(Motor1B, HIGH); 
  digitalWrite(Motor2A, LOW); 
  digitalWrite(Motor2B, LOW);
}

void Rotate_Right()
{
  digitalWrite(Motor1A, LOW); 
  digitalWrite(Motor1B, HIGH); 
  digitalWrite(Motor2A, HIGH); 
  digitalWrite(Motor2B, LOW); 
}

void Rotate_Left()
{
  digitalWrite(Motor1A, HIGH); 
  digitalWrite(Motor1B, LOW); 
  digitalWrite(Motor2A, LOW); 
  digitalWrite(Motor2B, HIGH); 
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
void FoundObstacle()
{
 Stop();
  Myservo.write(180); // right 
   UltrON();
   if(Distance<SafeZone)
   {
     Myservo.write(0);
          Rotate_Left();
          delay(5000); 
          Forword();
         while((Distance<SafeZone)) // must left 
         {
              UltrON();
         }
         delay(4000);
         Rotate_Right();
          delay(5000); 
          Forword();
         while((Distance<SafeZone)) // must left 
         {
              UltrON();
         }
         delay(7000);
         Rotate_Right();
         Forword();
         while(!digitalRead(IRSensor1) && !digitalRead(IRSensor2));
         Left();
         delay(3000);
 
   }
   
   else //right 
   {
      Myservo.write(180);
          Rotate_Right();
          delay(5000); 
          Forword();
         while((Distance<SafeZone)) // must left 
         {
              UltrON();
         }
         delay(4000);
         Rotate_Left();
          delay(5000); 
          Forword();
         while((Distance<SafeZone)) // must left 
         {
              UltrON();
         }
         delay(7000);
         Rotate_Left();
         Forword();
         while(!digitalRead(IRSensor1) && !digitalRead(IRSensor2));
         Right();
         delay(3000);
   }
       
}


//check again for revsion 
void FireFighting()
{

     for(Servopos=0;Servopos<=180;Servopos++)
        { 
           if(VoltRead(analogRead(FlamPin)) < 4 && VoltRead(analogRead(FlamPin)) > 1.5 )//if the lame pind read any thing during servo motion stop, get out the loop and open the pumb 
           {
            Serial.println("Found Fire!");
            break ;//get out 
           }    
            Myservo.write(Servopos);
            delay(200);
        }
    digitalWrite(PumbPin , HIGH);
             delay(5000);
   digitalWrite(PumbPin , LOW);   

             /*
  int CheckAgain = 0 ;
  while(CheckAgain <= 2)
  {
      Serial.println("Robot Stop");   
      // start to search and fight the fire here
        for(Servopos=0;Servopos<=180;Servopos++)
        { 
           if(VoltRead(analogRead(FlamPin)) < 4 && VoltRead(analogRead(FlamPin)) > 1.5 )//if the lame pind read any thing during servo motion stop, get out the loop and open the pumb 
           {
            Serial.println("Found Fire!");
            FirePostionshere = true ;
            CheckAgain = 0 ;
            break ;//get out 
           }    
            Myservo.write(Servopos);
            delay(200);
        }
        
        if(FirePostionshere)// in case found real fire 
        {
          while(VoltRead(analogRead(FlamPin)) <= 4.5 ) // don't get out from here or close the pumb untill the fire off 
          {
             digitalWrite(PumbPin , HIGH);
             delay(1000);
          }
        } 
         delay(5000);
        CheckAgain++ ;
        FirePostionshere = false ;
        digitalWrite(PumbPin , LOW); // cloase the water pumb 
        
  }//end check again loop 
  
          //check again about the fire 
          FirePostionshere = false ; // Re Init. all the values again 
          digitalWrite(PumbPin , LOW); // cloase the water pumb */
      
  
}

