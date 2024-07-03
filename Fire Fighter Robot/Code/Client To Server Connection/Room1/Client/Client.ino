//Client Code 
//Room 1
#define TCP_MSS whatever
#define LWIP_IPV6 whatever
#define LWIP_FEATURES whatever
#define LWIP_OPEN_SRC whatever

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti WiFiMulti;


#define Sensor1  D0
#define Sensor2  D1
#define Sensor3  D2
#define Sensor4  D3
#define Door     D4

bool DoneReq = true ;

int httpGETRequest(const char* serverName) ;

const char* ssid = "Robot";
const char* password = "E12345678";

const char* ssidNet = "TP-Link_2578";
const char* passwordNet = "25461626";

/*********** Set the Link for IFTTT *****************/
const char host[] = "maker.ifttt.com"; //server name 
const char privateKey[] = "d1lwdJfdu3MtEBug4sB--7RdjLXnsS3IxudVu47ngdB";
const char event[] = "Fire";
const char port = 80 ; // this post for the server 


//Your IP address or domain name with URL path
const char* serverName = "http://192.168.4.1/Fun1"; // this part for room 1 only and we will change it in room 2 
WiFiClient client ;
WiFiClient client2 ;

int httpGETRequest(const char* serverName) ;

void setup() 
{
  pinMode(Sensor1 , INPUT);
  pinMode(Door , OUTPUT);
  digitalWrite(Door , LOW);
  
  Serial.begin(115200);
  Serial.println();

   
}

void loop() 
{ 
  if(!DoneReq)
  {
    delay(50000);
    DoneReq = true ;
  }
  if(digitalRead(Sensor1) == HIGH && DoneReq)// || digitalRead(Sensor2) == HIGH || digitalRead(Sensor3) == HIGH  || digitalRead(Sensor4) == HIGH && DoneReq
  {
        Serial.print("Connecting to robot ");
        digitalWrite(Door , HIGH); // open the door 
        Serial.println(ssid);
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) 
        {
          delay(500);
          Serial.print(".");
        }
        Serial.println("connected");
    if ((WiFiMulti.run() == WL_CONNECTED)) //to make sure the wifi still here (Connected)
    {
         int httpResponseCode = httpGETRequest(serverName);//makw arquest to the server 
         if(httpResponseCode == 200) // so the request is doneso we need to wait for robot and open the door 
         {
            DoneReq = false ;
            

             //connect to the internet on the network
            Serial.print("Connecting to internet ");
            Serial.println(ssidNet);
            WiFi.begin(ssidNet, passwordNet);
            while (WiFi.status() != WL_CONNECTED) 
            {
              delay(500);
              Serial.print(".");
            }
            Serial.println("");
            Serial.println("Connected to WiFi");

            // before any thing we must connect to the server IFTTT 
            Serial.println("Connected To IFTTT Server");
            while(!client.connect(host , port))
            {
              Serial.print(".");
            }
            Serial.println("You Are Connected to the IFTTT Server");  
            //just we need to amke this request to trrige the sit to send  the sms masseage 
            HTTPClient http;
            http.begin(client2 , "http://maker.ifttt.com/trigger/Fire/with/key/d1lwdJfdu3MtEBug4sB--7RdjLXnsS3IxudVu47ngdB");
        
            int httpCode = http.GET();
            if (httpCode > 0) //200 for good response code 
            { //Check for the returning code       
              String payload = http.getString();
              Serial.println(httpCode);
              Serial.println(payload);
            }
            else 
            {
              Serial.println("Error on HTTP request");
            }
            
            http.end();          
            //after the request that mean we have beem sent the message to the client 
            Serial.print("SMS has been sent");
    /*********************************************************************************/ 
         }
    }
    else
    {
          Serial.println("WiFi Disconnected to the robot server ");
          WiFi.begin(ssid, password);
          while (WiFi.status() != WL_CONNECTED) 
          {
              Serial.print(".");
          }
    }
  }


}

int httpGETRequest(const char* serverName) 
{
  HTTPClient http;
    
  // Your IP address with path or Domain name with URL path 
  http.begin(client , serverName);
  
  // Send HTTP POST request
  int httpResponseCode = http.GET();

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);

  // Free resources
  http.end();

return httpResponseCode ;
}
