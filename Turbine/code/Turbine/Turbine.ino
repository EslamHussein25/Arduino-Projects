#include <LiquidCrystal.h>
#include "TimerOne.h" // Timer Interrupt set to 2 second for read sensors 
#include <math.h> 

#define WindSensorPin (2) // The pin location of the anemometer sensor 
#define WindVanePin (A4) // The pin the wind vane sensor is connected to 
#define VaneOffset 0; // define the anemometer offset from magnetic north 
#define M1 3
#define M2 4
#define M1A 11
#define M2A 12
const int 
RS = 5,
EN = 6,
D4 = 7,
D5 = 8,
D6 = 9,
D7 = 10;

LiquidCrystal LCD(RS, EN, D4, D5, D6, D7);

int vaneValue; // raw analog value from wind vane 
int windDirection; // translated 0 - 360 direction 
int windCalDirection; // converted value with offset applied 
String windCompassDirection; // wind direction as compass points
int lastWindDirectionValue; // last direction value 

// sensors to be added
int temperature;
int barometer; 


volatile bool IsSampleRequired; // this is set true every 2.5s. Get wind speed 
volatile unsigned int TimerCount; // used to determine 2.5sec timer count 
volatile unsigned long Rotations; // cup rotation counter used in interrupt routine 
volatile unsigned long ContactBounceTime; // Timer to avoid contact bounce in isr 

float WindSpeed; // speed miles per hour 
float WS = 0;

bool flag1 = true ;
bool flag2 = false ;
bool flag3 = true ;
bool flag4 = false ;


void setup() { 

  lastWindDirectionValue = 0; 
  
  IsSampleRequired = false; 
  
  TimerCount = 0; 
  Rotations = 0; // Set Rotations to 0 ready for calculations 
  
  Serial.begin(9600); 
  
  pinMode(WindSensorPin, INPUT); 
  pinMode(M1, OUTPUT); 
  pinMode(M2, OUTPUT); 

  pinMode(M1A, OUTPUT); 
  pinMode(M2A, OUTPUT); 
  STOP();
  STOP1();
  LCD.begin(16, 2);
  LCD.clear(); 
  LCD.setCursor(5,0);
  LCD.print("Welcome");
  delay(1000);
  LCD.clear(); 
  attachInterrupt(digitalPinToInterrupt(WindSensorPin), isr_rotation, FALLING); 
  
  Serial.println("Arduino Weather Station"); 
  
  // Setup the timer interupt 
  Timer1.initialize(250000);// Timer interrupt every 2.5 seconds 
  Timer1.attachInterrupt(isr_timer); 

} 



void loop() 
{ 

  getWindDirection(); 

  
  // Only update the display if change greater than 5 degrees. 
  if(abs(windCalDirection - lastWindDirectionValue) > 5) 
  { 
    lastWindDirectionValue = windCalDirection; 
  } 

  if(IsSampleRequired) 
  {
     
    // convert to mp/h using the formula V=P(2.25/T) 
    // V = P(2.25/2.5) = P * 0.9 
    WindSpeed = Rotations * 0.9; 
    Rotations = 0; // Reset count for next sample 
    
    IsSampleRequired = false; 

    WS = getKnots(WindSpeed) ;

    Serial.print("wind_speed="); Serial.print(WS); 
    Serial.println();
    Serial.print("wind_direction="); Serial.print(windCalDirection);  
    Serial.println();
    Serial.print("wind_compass_direction="); Serial.print(windCompassDirection); 
    Serial.println();
    LCD.setCursor(0,0);
    LCD.print("WS: ");
    LCD.setCursor(5,0);
    LCD.print(WS);
    LCD.setCursor(11,0);
    LCD.print("m/s");

    
    LCD.setCursor(0,1);
    LCD.print("WD: ");
    LCD.setCursor(5,1);
    LCD.print(windCalDirection);
    if(windCalDirection<100 && windCalDirection >9)
    {
      LCD.setCursor(7,1);
      LCD.print(" ");
    }
    else if(windCalDirection<10 && windCalDirection >0)
    {
      LCD.setCursor(7,1);
      LCD.print(" ");
      LCD.setCursor(6,1);
      LCD.print(" ");
    }
      LCD.setCursor(8,1);
      LCD.print(char(223));
      LCD.setCursor(11,1);
      LCD.print(windCompassDirection);
  //|| (windCalDirection <= 150 && windCalDirection >= 10 )
    if(flag1 && ((WS >= 3 && WS <= 5) || (WS >= 9 && WS <= 12) ) )
    {
      RIGHT();
      delay(3000);
      STOP();
      flag2 = true ;
      flag1 = false ;
    }
    //|| (windCalDirection <= 350 && windCalDirection >= 160 )
    else if(flag2 && ((WS >= 5 && WS <= 8 ) || (WS >= 1 && WS <= 2) ) )
    {
      LEFT();
      delay(3000);
      STOP();
      flag2 = false ;
      flag1 = true ;
    }
    else 
    {
      STOP();
    }

    
  if(flag3 && ((windCalDirection <= 150 && windCalDirection >= 100 ) || (windCalDirection <= 50 && windCalDirection >= 10 )) )
  {
      RIGHT1();
      delay(3000);
      STOP1();
      flag4 = true ;
      flag3 = false ;
  }
  else if (flag4 && ((windCalDirection <= 350 && windCalDirection >= 200) || (windCalDirection <= 100 && windCalDirection >= 60 )) )
  {
      LEFT1();
      delay(3000);
      STOP1();
      flag4 = false ;
      flag3 = true ;
  }
  else 
  {
    STOP1();
  }
    
  } 
} 


// isr handler for timer interrupt 
void isr_timer() 
{ 

  TimerCount++; 
  
  if(TimerCount == 6) 
  { 
    IsSampleRequired = true; 
    TimerCount = 0; 
  } 
} 




// This is the function that the interrupt calls to increment the rotation count 
void isr_rotation() 
{ 

  if((millis() - ContactBounceTime) > 15 ) 
  { // debounce the switch contact. 
    Rotations++; 
    ContactBounceTime = millis(); 
  } 

} 






// Get Wind Direction 
void getWindDirection() 
{ 

  vaneValue = analogRead(WindVanePin); 
  
  
  windCalDirection = windDirection + VaneOffset; 
  
  if(windCalDirection > 360) windCalDirection = windCalDirection - 360; 
  if(windCalDirection < 0) windCalDirection = windCalDirection + 360; 


  // get the compass direction for convenience
  if (windCalDirection < 22) windCompassDirection = "N";
  else if (windCalDirection < 67) windCompassDirection = "NE";
  else if (windCalDirection < 112) windCompassDirection = "E";
  else if (windCalDirection < 157) windCompassDirection = "SE";
  else if (windCalDirection < 212) windCompassDirection = "S";
  else if (windCalDirection < 247) windCompassDirection = "SW";
  else if (windCalDirection < 292) windCompassDirection = "W";
  else if (windCalDirection < 337) windCompassDirection = "NW";
  else windCompassDirection = "N";  

} 

//motor 1
void LEFT()
{
  digitalWrite(M1 , HIGH);
  digitalWrite(M2 , LOW);
}

void RIGHT()
{
  digitalWrite(M1 , LOW);
  digitalWrite(M2 , HIGH);
}

void STOP()
{
  digitalWrite(M1 , LOW);
  digitalWrite(M2 , LOW);
}
//motor 2 
void LEFT1()
{
  digitalWrite(M1A , HIGH);
  digitalWrite(M2A , LOW);
}

void RIGHT1()
{
  digitalWrite(M1A , LOW);
  digitalWrite(M2A , HIGH);
}

void STOP1()
{
  digitalWrite(M1A , LOW);
  digitalWrite(M2A , LOW);
}



// Convert MPH to Knots 
float getKnots(float speed) 
{ 
  return speed * 0.868976; 
} 
