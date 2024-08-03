#include <SoftwareSerial.h>

#define MOTOR1A  2
#define MOTOR1B  3

#define MOTOR2A  4
#define MOTOR2B  5

#define RX       8
#define TX       7

SoftwareSerial BlueCtrl(RX, TX); // rx with 8  and  tx with 7 

void Forword();
void RIGHT();
void LEFT();
void BACK();
void STOP();
void BREAK();

char Bluetoothdata; // to read the data from bluetooth 

void setup() 
{
  //motor 1 , 2 
pinMode(MOTOR1A , OUTPUT);
pinMode(MOTOR1B , OUTPUT);

 // motor 3 , 4
pinMode(MOTOR2A , OUTPUT);
pinMode(MOTOR2B , OUTPUT);
BlueCtrl.begin(9600); // buad rate and start communication between arduino and bluetooth 
}

void loop() 
{

  if(BlueCtrl.available())
  {
    Bluetoothdata = BlueCtrl.read();
    if(Bluetoothdata == 'F')
    {
      Forword();
    }
    else if (Bluetoothdata == 'B')
    {
      BACK();
    }
    else if (Bluetoothdata == 'L')
    {
      LEFT();
    }
    else if (Bluetoothdata == 'R')
    {
      RIGHT();
    }
    else if (Bluetoothdata == 'S')
    {
      STOP();
    }
    else if (Bluetoothdata == 'W')
    {
      BREAK();
    }
  }

}


void Forword()
{
    digitalWrite(MOTOR1A ,LOW); 
    digitalWrite(MOTOR1B ,HIGH);
    
    digitalWrite(MOTOR2A ,LOW);
    digitalWrite(MOTOR2B ,HIGH);
}

void RIGHT()
{
    digitalWrite(MOTOR1A ,HIGH); 
    digitalWrite(MOTOR1B ,LOW);
    digitalWrite(MOTOR2A ,LOW);
    digitalWrite(MOTOR2B ,HIGH);
}

void LEFT()
{
    digitalWrite(MOTOR1A ,LOW); 
    digitalWrite(MOTOR1B ,HIGH);
    digitalWrite(MOTOR2A ,HIGH);
    digitalWrite(MOTOR2B ,LOW);
}

void BACK()
{
    digitalWrite(MOTOR1A ,HIGH); 
    digitalWrite(MOTOR1B ,LOW);
    digitalWrite(MOTOR2A ,HIGH);
    digitalWrite(MOTOR2B ,LOW);
}

void STOP()
{
    digitalWrite(MOTOR1A ,LOW); 
    digitalWrite(MOTOR1B ,LOW);
    digitalWrite(MOTOR2A ,LOW);
    digitalWrite(MOTOR2B ,LOW);
}

void BREAK()
{
    digitalWrite(MOTOR1A ,HIGH); 
    digitalWrite(MOTOR1B ,HIGH);
    digitalWrite(MOTOR2A ,HIGH);
    digitalWrite(MOTOR2B ,HIGH);
}






