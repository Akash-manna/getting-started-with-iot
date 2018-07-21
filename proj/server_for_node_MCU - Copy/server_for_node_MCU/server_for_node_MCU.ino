/*  Connects to the home WiFi network
 *  Asks some network parameters
 *  Starts WiFi server with fix IP and listens
 *  Receives and sends messages to the client
 *  Communicates: client for node MCU.ino
 *  Author - Shubham Sinha
 */
#include <SPI.h>
#include <ESP8266WiFi.h>

int led = D4;
char ssid[] = "vivo Y21";               // SSID of your home WiFi
char pass[] = "123456789a";               // password of your home WiFi
WiFiServer server(80);                    

//IPAddress ip(192, 168, 0, 80);            // IP address of the server
//IPAddress gateway(192,168,0,1);           // gateway of your network
//IPAddress subnet(255,255,255,0);          // subnet mask of your network

void setup() {
  Serial.begin(115200);                   // only for debug
  //WiFi.config(ip, gateway, subnet); // forces to use the fix IP

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, pass);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
  
  //server.begin();                         // starts the server
// Serial.println("Connected to wifi");
//  Serial.print("Status: "); Serial.println(WiFi.status());  // some parameters from the network
//  Serial.print("IP: ");     Serial.println(WiFi.localIP());
//  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
//  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
//  Serial.print("SSID: "); Serial.println(WiFi.SSID());
//  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
//  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());
  pinMode(led, OUTPUT);
}

void loop () {
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      digitalWrite(led, LOW);  // to show the communication only (inverted logic)
      Serial.println(".");
      String request = client.readStringUntil('\r');    // receives the message from the client
      Serial.print("From client: "); 
      Serial.println(request);
      client.flush();
      Serial.println("Hi client! No, I am listening.\r"); // sends the answer to the client
      digitalWrite(led, HIGH);
    }
    client.stop();                // tarminates the connection with the client
  }
}
