#include<ESP8266WiFi.h>
#include<WiFiUdp.h>

const char* ssid ="Netbank";
const char* password="rud123456";

unsigned int localport = 59999;
WiFiUDP Udp;
char ReceivingBuffer[UDP_TX_PACKET_MAX_SIZE];
void setup() {
  // put your setup code here, to run once:
  pinMode(2,OUTPUT);
  Serial.begin(115200);
  delay(100);

  WiFi.begin(ssid,password);
  Serial.print("Connecting to->->->->->->");
  Serial.println(ssid);

  while(WiFi.status()  !=WL_CONNECTED)
  {
    delay(1200);
    Serial.print("*");
  }

  Serial.println("");
  Serial.println("Wifi Connected->->");
  WiFi.begin();
  Serial.println("Server Started");
  Serial.println(WiFi.localIP());
  }


void loop() {
  // put your main code here, to run repeatedly:
WiFiClient client = WiFi.available();
if (!client) {
 return;
}
while(!client.available()){
 delay(1);
}
rest.handle(client);
}
