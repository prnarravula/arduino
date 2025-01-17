#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <DHT.h>

#define TRIG_PIN 5
#define ECHO_PIN 18
#define DHT_SENSOR_PIN 23
#define DHT_SENSOR_TYPE DHT11

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

const char* ssid = "";
const char* password = "";
float duration_us, distance_cm;
int  sensorPin  =  36;

AsyncWebServer server(80);

String readD() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration_us = pulseIn(ECHO_PIN, HIGH);
  distance_cm = 0.017 * duration_us;
  if (isnan(distance_cm)) {    
    Serial.println("Failed to read!");
    return "";
  }
  else {
    Serial.println(distance_cm);
    return String(distance_cm);
  }
}

String readH() {
  float humi  = dht_sensor.readHumidity();
  if (isnan(humi)) {
    Serial.println("Failed to read!");
    return "";
  }
  else {
    Serial.println(humi);
    return String(humi);
  }
}

String readT() {
  float tempF = dht_sensor.readTemperature(true);
  if (isnan(tempF)) {
    Serial.println("Failed to read!");
    return "";
  }
  else {
    Serial.println(tempF);
    return String(tempF);
  }
}

String readL() {
  int sensorValue = analogRead(sensorPin);
  if (isnan(sensorValue)) {
    Serial.println("Failed to read!");
    return "";
  }
  else {
    Serial.println(sensorValue);
    return String(sensorValue);
  }
}

void setup(){
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  dht_sensor.begin();
  
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readD().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readH().c_str());
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readT().c_str());
  });
  server.on("/light", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readL().c_str());
  });
  

  server.begin();
}
 
void loop(){
  
}
