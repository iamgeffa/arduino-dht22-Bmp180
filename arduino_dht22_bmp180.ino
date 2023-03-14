#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085.h>
#include <DHT.h>

// Pin for the DHT22 sensor
#define DHTPIN 3

// Type of sensor - DHT22
#define DHTTYPE DHT22

// Create a DHT22 object
DHT dht(DHTPIN, DHTTYPE);

// Create a BMP180 object
Adafruit_BMP085 bmp;

void setup() {
  // Start serial communication
  Serial.begin(9600);
  delay(2000); // Wait for the serial port to initialize

  // Initialize the DHT22 sensor
  dht.begin();

  // Initialize the BMP180 sensor
  if (!bmp.begin()) {
    Serial.println("Failed to initialize BMP180 sensor!");
    while (1);
  }
}

void loop() {
  // Read temperature and humidity data from the DHT22 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read temperature and pressure data from the BMP180 sensor
  float bmp_temperature = bmp.readTemperature();
  float pressure = bmp.readPressure();
  float altitude = bmp.readAltitude(1013.25); // Calculate altitude relative to sea level

  // Print the readings to the serial monitor
  Serial.print("DHT22 - ");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");

  Serial.print("BMP180 - ");
  Serial.print("Temperature: ");
  Serial.print(bmp_temperature);
  Serial.print(" °C, Pressure: ");
  Serial.print(pressure/100.0);
  Serial.print(" kPa, Altitude: ");
  Serial.print(altitude);
  Serial.println(" m");

  // Wait for 2 seconds before taking the next reading
  delay(2000);
}
