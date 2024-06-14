#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// WiFi Configuration
const char* ssid = ";tim11";
const char* password = "987654321";

// Sensor configuration
const int sensorPin = 4;
const String sensorName = "suhu";

// RestAPI Configuration
const char* baseRoute = "192.168.43.33:5000/sensor/";
const int requestDelay = 1000;

DHT dht(sensorPin, DHT11);

void setup() {
  Serial.begin(9600);
  dht.begin();

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi network");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(requestDelay);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;

    http.begin(client, baseRoute+sensorName);
    http.addHeader("Content-Type", "application/json");
    int statusCode = http.POST("{\"celcius\": "+String(t)+", \"fahrenheit\": "+String(f)+", \"humidity\": "+String(h)+"}");

    Serial.println(statusCode);
    http.end();
  }else{
    Serial.println("WiFi Disconnected");
  }
}