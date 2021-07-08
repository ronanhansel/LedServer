#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <SoftwareSerial.h> //Included SoftwareSerial Library
SoftwareSerial s(3, 1);

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>
#define RED_PIN 14 //D5
#define GREEN_PIN 12 //D6
#define BLUE_PIN 13 //D7
//On board LED Connected to GPIO2
#define LED 2  
String data = "";
//SSID and Password of your WiFi router
const char* ssid = "Nhà của Cún :L";
const char* password = "84568456";
//int r = 0;
//int g = 0;
//int b = 0;
//double brief = 1;

//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80
//---------------------------------------------------------------
//Our HTML webpage contents in program memory
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<body>
<center>
<h1>WiFi LED on off demo: 1</h1><br>
Ciclk to turn <a href="ledOn">LED ON</a><br>
Ciclk to turn <a href="ledOff">LED OFF</a><br>
<hr>
</center>

</body>
</html>
)=====";

//---------------------------------------------------------------
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
// Serial.println("You called root page");
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleLEDon() { 
// Serial.println("LED on page");
 digitalWrite(LED,LOW); //LED is connected in reverse
 server.send(200, "text/html", "LED is ON"); //Send ADC value only to client ajax request
}

void handleLEDoff() { 
// Serial.println("LED off page");
 digitalWrite(LED,HIGH); //LED off
 server.send(200, "text/html", "LED is OFF"); //Send ADC value only to client ajax request
}

void handleUpdate() {
  data = server.arg("data");  
  s.println(data);
  server.send(200, "text/plain", "Updated");
}

//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  s.begin(9600);
  pinMode(RED_PIN,OUTPUT); 
  pinMode(GREEN_PIN,OUTPUT); 
  pinMode(BLUE_PIN,OUTPUT); 
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  //Onboard LED port Direction output
  pinMode(LED,OUTPUT); 
  //Power on LED state off
  digitalWrite(LED,HIGH);
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  digitalWrite(LED,LOW);
  IPAddress local_IP(192,168,1,69);
  IPAddress gateway(192,168,1,1);
  IPAddress subnet(255,255,255,0);
  IPAddress primaryDNS(1,1,1,1);
  IPAddress secondaryDNS(1,0,0,1);
  digitalWrite(LED,LOW);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
  Serial.println("STA Failed to configure");
  }
  
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/ledOn", handleLEDon); //as Per  <a href="ledOn">, Subroutine to be called
  server.on("/ledOff", handleLEDoff);
  server.on("/change", handleUpdate);

  server.begin();                  //Start server
  Serial.println("HTTP server started");
}



//==============================================================
//                     LOOP
//==============================================================
void loop(void) {
  server.handleClient();          //Handle client requests
}
