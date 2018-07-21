#include<ESP8266WiFi.h>

const char* ssid="vivo Y21";
const char* password="123456789";

WiFiServer server(80);
void setup() {

  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(115200);
  delay(100);

  WiFi.begin(ssid,password);
  Serial.println("Connecting to-----");
  Serial.println(ssid);

  while(WiFi.status()  !=WL_CONNECTED)
  {
    delay(1200);
    Serial.println("Retrying connection");
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  server.begin();
  Serial.println(WiFi.localIP());
  
    
  
  // put your setup code here, to run once:

}

void loop() {
  
  
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if(!client)
  {
    return;
  }
  Serial.println("New client");
  while(!client.available())
  {
    delay(1);
  }
  String request=client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  int value= LOW;
  if(request.indexOf("/LED=ON")  !=-1){
    digitalWrite(LED_BUILTIN,HIGH);
    value=HIGH;
  }
  if(request.indexOf("/LED=OFF")  !=-1)
  {
    digitalWrite(LED_BUILTIN,LOW);
    value=LOW;
  }
  client.println("HTTP/1.1 200 OK");
  client.println("Content_Type:text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  if(value==HIGH)
  {
    client.print("Pin is On");
  }
  else
  {
    client.print("Pin is OFF");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Turn ON</button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off</button></a><br />");
  client.println("</html>");
  delay(1);
  Serial.println("Client Disconnected");
  
    
    
  
  
  }
