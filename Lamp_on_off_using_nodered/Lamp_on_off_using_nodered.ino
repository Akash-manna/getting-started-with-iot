#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHT.h"

// Uncomment one of the lines bellow for whatever DHT sensor type you're using!
#define DHTTYPE DHT11   // DHT 11

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "vivo Y21";
const char* password = "one to nine triple b single a";

// Change the variable to your Raspberry Pi IP address, so it connects to your MQTT broker
const char* mqtt_server = "broker.mqtt-dashboard.com";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running in your home automation system
WiFiClient espClient;
PubSubClient client(espClient);

// DHT Sensor - GPIO 5 =  D1 on ESP-12E NodeMCU board
const int DHTPin =5;

// Lamp - LED - GPIO 4 = D2 on ESP-12E NodeMCU board
const int lamp = 4;

  // Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

// Timers auxiliar variables
long now = millis();
long lastMeasure = 0;

// Don't change the function below. This functions connects your ESP8266 to your router
void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

// This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
// Change the function below to add logic to your program, so when a device publishes a message to a topic that 
// your ESP8266 is subscribed you can actually do something
void callback(String topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
//  String autoManual;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();
/*  for(int i=0; i< length; i++){
      Serial.print((char)message[i]);
      autoManual += (char)message[i];
  }*/
//  Serial.print();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic akash/room/lamp, you check if the message is either on or off. Turns the lamp GPIO according to the message
 // float t = dht.readTemperature();
//  client.publish("akash/room/temperature", t);
  if(topic== "akash/room/lamp") { 
    Serial.print("Changing Room lamp to ");
    if(messageTemp == "on"){
      digitalWrite(lamp, HIGH);
      Serial.print("On");
    }
    else if(messageTemp == "off"){
      digitalWrite(lamp, LOW);
      Serial.print("off");
    }
  }
 /*     if (topic == "akash/room/lamp"){
        if (autoManual == "manual"){
          Serial.print("lamp is in manual");
          if (topic =="akash/room/lamp/manual"){
            Serial.print("Changing Room Lamp to ");
            if(messageTemp == "on"){
              digitalWrite(lamp, HIGH);
              Serial.print ("on");
            }
            else if(messageTemp == "off"){
              digitalWrite(lamp, LOW);
              Serial.print ("off");
              }
            }
          }
          else if (autoManual == "auto"){
            Serial.print("lamp is in auto");
        if(topic=="akash/room/lamp/auto"){
          Serial.print("Changing Room lamp to ");
          float t = dht.readTemperature();
          if(t>=31){
            digitalWrite(lamp, HIGH);
            Serial.print("On");
            client.publish("akash/room/lamp/auto", "Auto On");
          }
          else{
            digitalWrite(lamp, LOW);
            Serial.print("Off");
            client.publish("akash/room/lamp/auto", "Auto Off");
          }
        } 
      }
    }*/ 
/*}
void auto_on
  if(topic == "akash/room/lamp") {
      Serial.println("Changing lamp to");
      float t = dht.readTemperature();
      if(t <= 25){
          digitalWrite(lamp, HIGH);
          Serial.print("Auto-on");
      }
      else{
          digitalWrite(lamp, LOW);
          Serial.print("Auto-on");
        }
        }*/
  Serial.println();
}

// This functions reconnects your ESP8266 to your MQTT broker
// Change the function below if you want to subscribe to more topics with your ESP8266 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    /*
     YOU MIGHT NEED TO CHANGE THIS LINE, IF YOU'RE HAVING PROBLEMS WITH MQTT MULTIPLE CONNECTIONS
     To change the ESP device ID, you will have to give a new name to the ESP8266.
     Here's how it looks:
       if (client.connect("ESP8266Client")) {
     You can do it like this:
       if (client.connect("ESP1_Office")) {
     Then, for the other ESP:
       if (client.connect("ESP2_Garage")) {
      That should solve your MQTT multiple connections problem
    */
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.publish("akash/node/status", "connected");
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("akash/room/lamp");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in a second");
      // Wait 1 seconds before retrying
      delay(1000);
    }
  }
}

// The setup function sets your ESP GPIOs to Outputs, starts the serial communication at a baud rate of 115200
// Sets your mqtt broker and sets the callback function
// The callback function is what receives messages and actually controls the LEDs
void setup() {
  pinMode(lamp, OUTPUT);
  
  dht.begin();
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

// For this project, you don't need to change anything in the loop function. Basically it ensures that you ESP is connected to your broker
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  if(!client.loop())
    client.connect("ESP8266Client");

  now = millis();
  // Publishes new temperature and humidity every 2 seconds
  if (now - lastMeasure > 2000) {
    lastMeasure = now;
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    float f = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Computes temperature values in Celsius
    float hic = dht.computeHeatIndex(t, h, false);
    static char temperatureTemp[7];
    dtostrf(hic, 6, 2, temperatureTemp);
    
    // Uncomment to compute temperature values in Fahrenheit 
    // float hif = dht.computeHeatIndex(f, h);
    // static char temperatureTemp[7];
    // dtostrf(hic, 6, 2, temperatureTemp);
    
    static char humidityTemp[7];
    dtostrf(h, 6, 2, humidityTemp);

    // Publishes Temperature and Humidity values
    client.publish("akash/room/temperature", temperatureTemp);
    client.publish("akash/room/humidity", humidityTemp);
    
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t Temperature: ");
    Serial.print(t);
    Serial.print(" *C ");
    Serial.print(f);
    Serial.print(" *F\t Heat index: ");
    Serial.print(hic);
    Serial.println(" *C ");
    // Serial.print(hif);
    // Serial.println(" *F");
  }
} 
