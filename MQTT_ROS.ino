
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char *ssid = ""; // Insertar contraseña de la red Wi-Fi
const char *password = "";  // Contraseña de la red Wi-Fi a conectarse (si es publica dejar vacio)

// Broker MQTT, llenar con los datos relevantes del broker escogido.
const char *mqtt_broker = "";
const char *topic = "";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 1884;

// Motor A
int motorAPin1 = 19; 
int motorAPin2 = 21; 
int enableAPin = 18; 

// Motor B
int motorBPin3 = 22; 
int motorBPin4 = 23; 
int enableBPin = 17; 

// Propiedades PWM 
const int freq = 30000;
const int pwmChannelA = 0;
const int pwmChannelB = 1;
const int resolution = 8;
int dutyCycle = 200;
char firstChar;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
 Serial.begin(115200);
 //Conexion red Wi-Fi
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Conectando al WiFi..");
 }
 Serial.println("Conectado a la red Wi-Fi.");
 //Conexion al broker MQTT
 client.setServer(mqtt_broker, mqtt_port);
 client.setCallback(callback);
 while (!client.connected()) {
     String client_id = ""; //insertar el id del broker mqtt que se va a usar
     client_id += String(WiFi.macAddress());
     Serial.printf("El cliente %s se conecta al broker mqtt \n", client_id.c_str());
     if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("Conectado al broker MQTT exitosamente");
     } else {
         Serial.print("Fallo con el estado: ");
         Serial.print(client.state());
         delay(2000);
     }
 }
 // Publicar y subscribirse al topico
 client.publish(topic, "ESP32 conectada al servidor MQTT.");
 client.subscribe(topic);
// Colocar los pines como salida
  pinMode(motorAPin1, OUTPUT);
  pinMode(motorAPin2, OUTPUT);
  pinMode(enableAPin, OUTPUT);
  pinMode(motorBPin3, OUTPUT);
  pinMode(motorBPin4, OUTPUT);
  pinMode(enableBPin, OUTPUT);
  
  // Configurar la funcionalidad del PWM
  ledcSetup(pwmChannelA, freq, resolution);
  ledcSetup(pwmChannelB, freq, resolution);
  
  // Asignar el canal al pin del GPIO para el PWM
  ledcAttachPin(enableAPin, pwmChannelA);
  ledcAttachPin(enableBPin, pwmChannelB);
 
}

void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Mensaje nuevo en el topico: ");
 Serial.println(topic);
 Serial.print("Mensaje:");
 for (int i = 0; i < length; i++) {
     Serial.print((char) payload[i]);
     if(i == 0){
      firstChar = (char)payload[i];
      }
 }
}

void loop() {
 client.loop();
 switch(firstChar){
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
