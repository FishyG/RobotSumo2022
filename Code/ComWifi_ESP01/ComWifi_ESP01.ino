#include <ESP8266WiFi.h>
#include <WiFiClient.h>


int port = 8888;  //Port number
WiFiServer server(port);

//Server connect to WiFi Network
const char *ssid = "";  //Enter your wifi SSID
const char *password = "";  //Enter your wifi Password

void setup() 
{
  Serial.begin(115200);
  Serial1.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to wifi
 
  // Wait for connection  
  Serial.println("Connecting to Wifi");
   // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.print("Open a connection to IP:");
  Serial.print(WiFi.localIP());
  Serial.print(" on port ");
  Serial.println(port);
}

void loop() 
{
  WiFiClient client = server.available();
  
  if (client) {
    if(client.connected())
    {
      Serial.println("Client Connected");
        
      while(client.connected())
      {      
        while(client.available()>0)
        {
          // read data from the connected client
          Serial1.write(client.read()); 
          Serial.println("Data was received, sending it to the serial port 1. You are now on serial port 0");
          //Send Data to connected client
          client.println("Data was received, sending it to the serial port 1. You are now on a TCP connection");
        }
        
         //Send Data to connected client
        while(Serial.available()>0)
        {
          client.write(Serial.read());
        }
      }
      client.stop();
    }
    
    Serial.println("");
    Serial.println("Client disconnected");    
  }
}