 
int ctrl1 = 2 ;
int ctrl2 = 4;
int pwm1 = 3;
int pwm2 = 5 ;

int Armctrl = 6 ;

int Incoming_Direction = 0;                //Variable for storing Incoming_value

void setup() 
{
//motors control pins 
pinMode(ctrl1,OUTPUT);  
pinMode(ctrl2,OUTPUT);   

pinMode(pwm1 , OUTPUT);
pinMode(pwm2 , OUTPUT);
digitalWrite(pwm1 , LOW);
digitalWrite(pwm2 , LOW);

//arm control pin 
pinMode(Armctrl , OUTPUT);
digitalWrite(Armctrl , LOW);

//Serila pin for bluetooth 
Serial.begin(9600);         //Sets the data rate in bits per second (baud) for serial data transmission

}


void loop()
{
  
  if(Serial.available() > 0)  
  {
    
   Incoming_Direction = Serial.read();      //Read the incoming data and store it into variable Incoming_value

      if(Incoming_Direction == 'F')
      {
            while(Incoming_Direction == 'F')
            {
              Forword();         
              Incoming_Direction = Serial.read(); 
            }
            Stop();
      }
      else if (Incoming_Direction == 'R')
      {
           while(Incoming_Direction == 'R')
            {
              Right();  
              Incoming_Direction = Serial.read(); 
            }
            Stop();
    
      }
      else if (Incoming_Direction == 'L')
      {
           while(Incoming_Direction == 'L')
            {
              Left(); 
              Incoming_Direction = Serial.read(); 
            }
            Stop();  
      }
      else if (Incoming_Direction == 'B')
      {
             while(Incoming_Direction == 'B')
            {
              Back();  
            
              Incoming_Direction = Serial.read(); 
            }
            Stop();
      }
      
      else if (Incoming_Direction == 'S')//stop  - light on 
      {
            Stop();
      }





       if (Incoming_Direction == 'W')//OPen  
      {
        ArmOpen();
      }

      else if (Incoming_Direction == 'w')//Close  
      {
       ArmClose();
      }
                  
  }
                         
}



//function part 

void Forword()
{
      digitalWrite(pwm1 , HIGH);
      digitalWrite(pwm2 , HIGH);
      digitalWrite(ctrl1,LOW);   //left motors forward
      digitalWrite(ctrl2,LOW);   //left motors reverse
}

void Left()
{
        //Move Left 
      digitalWrite(pwm1 , HIGH);
      digitalWrite(pwm2 , HIGH);
      digitalWrite(ctrl1,HIGH);   //left motors forward
      digitalWrite(ctrl2,LOW);   //left motors reverse
}

void Right()
{
         //Move Right 
      digitalWrite(pwm1 , HIGH);
      digitalWrite(pwm2 , HIGH);
      digitalWrite(ctrl1,LOW);   //left motors forward
      digitalWrite(ctrl2,HIGH);   //left motors reverse

}

void Back()
{
        //Move Back
      digitalWrite(pwm1 , HIGH);
      digitalWrite(pwm2 , HIGH);
      digitalWrite(ctrl1,HIGH);   //left motors forward
      digitalWrite(ctrl2,HIGH);   //left motors reverse
}

void Stop()
{
     //break
      digitalWrite(pwm1 , LOW);
      digitalWrite(pwm2 , LOW);
      digitalWrite(ctrl1,LOW);   //left motors forward
      digitalWrite(ctrl2,LOW);   //left motors reverse
}



void ArmOpen()
{
  digitalWrite(Armctrl , HIGH);
  delay(500);
}

void ArmClose()
{
   digitalWrite(Armctrl , LOW);
   delay(500);
}

