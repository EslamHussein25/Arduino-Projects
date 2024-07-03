// Server 

#define TCP_MSS whatever
#define LWIP_IPV6 whatever
#define LWIP_FEATURES whatever
#define LWIP_OPEN_SRC whatever

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define Room1Send D0
#define Room2Send D2

// Set your access point network credentials
const char* ssid = "Robot";
const char* password = "E12345678";

// Create AsyncWebServer object on port 80
ESP8266WebServer server(80);//port 

void Fun1();
void Fun2();

void setup()
{
  // Serial port for debugging purposes
  pinMode(Room1Send , OUTPUT);
  pinMode(Room2Send , OUTPUT);
  digitalWrite(Room1Send , LOW);
  digitalWrite(Room2Send , LOW);
  Serial.begin(115200);
  Serial.println();
  
  // Setting the ESP as an access point
  Serial.print("Setting AP (Access Point)…");
  // Remove the password parameter, if you want the AP (Access Point) to be open
  while(WiFi.softAP(ssid, password) == false)
  {
   Serial.print("."); 
  }

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
//if we write the root  of the server only will be make the fun1 request directly
  server.on("/" , Fun1); // handle requset and the / mean the server rooy without server function like .com so we can put "/" only 
  server.on("/Fun1" , Fun1);//room 1
  server.on("/Fun2" , Fun2);//room 2
 
   
  
  // Start server
  server.begin();
}
 
void loop()
{
  digitalWrite(Room1Send , LOW);
  digitalWrite(Room2Send , LOW);
  server.handleClient();
}

void Fun1()
{
  server.send(200 , "text/plain" , "Fire found in room 1");
  Serial.print("Req 1");
  digitalWrite(Room1Send , HIGH);
  delay(500);
  digitalWrite(Room1Send , LOW);
}

void Fun2()
{
  server.send(200 , "text/plain" , "Fire found in room 2");
  Serial.print("Req 2");
    digitalWrite(Room2Send , HIGH);
  delay(500);
  digitalWrite(Room2Send , LOW);
}
