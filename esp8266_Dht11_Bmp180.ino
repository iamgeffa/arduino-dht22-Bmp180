#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Adafruit_BMP085.h>

#define DHTPIN 2          // DHT22 signal pin
#define DHTTYPE DHT11     // DHT22 sensor type

// BMP180 sensor settings
Adafruit_BMP085 bmp;

// WiFi credentials
const char* ssid = "mustafa";
const char* password = "12345678";

// ThingSpeak settings
const char* server = "api.thingspeak.com";
const String apiKey = "42UBPUIRP309VXTQ";
const String field1Name = "Temperature";
const String field2Name = "Humidity";
const String field3Name = "Pressure";
const String field4Name = "Altitude";

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize DHT22 sensor
  dht.begin();

  // Initialize BMP180 sensor
  if (!bmp.begin()) {
    Serial.println("Failed to initialize BMP180 sensor!");
  }
}

void loop() {
  // Read temperature, humidity, pressure, and altitude from sensors
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float pressure = bmp.readPressure() / 100.0;
  float altitude = bmp.readAltitude();

  // Check if any reading failed and exit early
  if (isnan(temperature) || isnan(humidity) || isnan(pressure) || isnan(altitude)) {
    Serial.println("Failed to read from sensors!");
    return;
  }

  // Print readings to serial monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %, Pressure: ");
  Serial.print(pressure);
  Serial.print(" hPa, Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");

  // Send data to ThingSpeak
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(temperature);
    postStr += "&field2=";
    postStr += String(humidity);
    postStr += "&field3=";
    postStr += String(pressure);
    postStr += "&field4=";
    postStr += String(altitude);

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.println("Data sent to ThingSpeak");
  } else {
    Serial.println("Failed to connect to ThingSpeak");
  }

  // Wait for 20 seconds before sending the next data to ThingSpeak
  delay(20000);
}
