
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = "UMNG_PUB"; // Enter your WiFi name
const char *password = "";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "esp32_ros";
const char *mqtt_username = "jaimito";
const char *mqtt_password = "123456";
const int mqtt_port = 1883;

// Motor A
int motorAPin1 = 19; 
int motorAPin2 = 21; 
int enableAPin = 18; 

// Motor B
int motorBPin3 = 22; 
int motorBPin4 = 23; 
int enableBPin = 17; 

// Setting PWM properties 
const int freq = 30000;
const int pwmChannelA = 0;
const int pwmChannelB = 1;
const int resolution = 8;
int dutyCycle = 200;
char primerChar;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
 // Set software serial baud to 115200;
 Serial.begin(115200);
 // connecting to a WiFi network
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network");
 //connecting to a mqtt broker
 client.setServer(mqtt_broker, mqtt_port);
 client.setCallback(callback);
 while (!client.connected()) {
     String client_id = "mqttx_92b495d6";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("Public emqx mqtt broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
 }
 // publish and subscribe
 client.publish(topic, "ESP32 conectada al servidor MQTT.");
 client.subscribe(topic);
// sets the pins as outputs:
  pinMode(motorAPin1, OUTPUT);
  pinMode(motorAPin2, OUTPUT);
  pinMode(enableAPin, OUTPUT);
  pinMode(motorBPin3, OUTPUT);
  pinMode(motorBPin4, OUTPUT);
  pinMode(enableBPin, OUTPUT);
  
  // configure LED PWM functionalities
  ledcSetup(pwmChannelA, freq, resolution);
  ledcSetup(pwmChannelB, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(enableAPin, pwmChannelA);
  ledcAttachPin(enableBPin, pwmChannelB);
 
}

void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
 Serial.println(topic);
 Serial.print("Message:");
 for (int i = 0; i < length; i++) {
     Serial.print((char) payload[i]);
     if(i == 0){
      primerChar = (char)payload[i];
      }
 }
}

void loop() {
 client.loop();
 switch(primerChar){
    case 'S':
    Serial.print("Para");
      Parar();
    break;
    case 'F':
    Serial.print("Adelante");
      Adelante();
      setPWMSpeed(dutyCycle);
    break;
    case 'B':
      Atras();
      Serial.print("Atras");
      setPWMSpeed(dutyCycle);
    break;
    case 'L':
    Serial.print("Izquierda");
      Izquierda();
      setPWMSpeed(dutyCycle);
    break;
    case 'R':
    Serial.print("Derecha");
      Derecha();
      setPWMSpeed(dutyCycle);
    break;
    default:
    break;
  }
}

void Parar() {
  digitalWrite(motorAPin1, LOW);
  digitalWrite(motorAPin2, LOW);
  digitalWrite(motorBPin3, LOW);
  digitalWrite(motorBPin4, LOW);
}

void Atras() {
  digitalWrite(motorAPin1, HIGH);
  digitalWrite(motorAPin2, LOW);
  digitalWrite(motorBPin3, HIGH);
  digitalWrite(motorBPin4, LOW);
}

void Adelante() {
  digitalWrite(motorAPin1, LOW);
  digitalWrite(motorAPin2, HIGH);
  digitalWrite(motorBPin3, LOW);
  digitalWrite(motorBPin4, HIGH);
}

void Izquierda() {
  digitalWrite(motorAPin1, LOW);
  digitalWrite(motorAPin2, HIGH);
  digitalWrite(motorBPin3, HIGH);
  digitalWrite(motorBPin4, LOW);
}

void Derecha() {
  digitalWrite(motorAPin1, HIGH);
  digitalWrite(motorAPin2, LOW);
  digitalWrite(motorBPin3, LOW);
  digitalWrite(motorBPin4, HIGH);
}

void setPWMSpeed(int speed) {
  ledcWrite(pwmChannelA, speed);   
  ledcWrite(pwmChannelB, speed);
}
