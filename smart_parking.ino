#include <WiFi.h>
#include <HTTPClient.h>

// ===============================
// WiFi
// ===============================
const char* ssid = "CirkitWifi";
const char* password = "";

// ===============================
// ThingSpeak
// ===============================
const char* thingSpeakAPIKey = "YOUR_THINGSPEAK_WRITE_API_KEY";

// ===============================
// Ultrasonic Sensors
// ===============================
#define TRIG1 5
#define ECHO1 18

#define TRIG2 16
#define ECHO2 17

#define PARKING_LIMIT 20

// ThingSpeak minimum update interval
unsigned long lastThingSpeakUpdate = 0;
const unsigned long thingSpeakInterval = 15000;

// ===============================
// Get Distance
// ===============================
float getDistance(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0)
  {
    return 100.0;
  }

  float distance = duration * 0.0343 / 2;

  if (distance > 100)
  {
    distance = 100;
  }

  return distance;
}

// ===============================
// Send Data to ThingSpeak
// ===============================
void sendToThingSpeak(float distance1,
                      float distance2,
                      int availableSlots,
                      int occupiedSlots)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi not connected!");
    return;
  }

  HTTPClient http;

  String url = "http://api.thingspeak.com/update?api_key=";
  url += thingSpeakAPIKey;
  url += "&field1=";
  url += String(distance1, 2);
  url += "&field2=";
  url += String(distance2, 2);
  url += "&field3=";
  url += String(availableSlots);
  url += "&field4=";
  url += String(occupiedSlots);

  http.begin(url);

  int httpResponseCode = http.GET();

  Serial.print("ThingSpeak Response: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode > 0)
  {
    Serial.print("ThingSpeak Entry ID: ");
    Serial.println(http.getString());
  }
  else
  {
    Serial.println("Failed to send data to ThingSpeak");
  }

  http.end();
}

// ===============================
// Setup
// ===============================
void setup()
{
  Serial.begin(115200);

  pinMode(TRIG1, OUTPUT);
  pinMode(ECHO1, INPUT);

  pinMode(TRIG2, OUTPUT);
  pinMode(ECHO2, INPUT);

  Serial.println();
  Serial.println("===== SMART PARKING SYSTEM =====");

  Serial.println("Connecting to WiFi...");

  WiFi.begin(ssid, password);

  unsigned long startTime = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - startTime < 10000)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("WiFi connection failed!");
    Serial.println("Sensor monitoring will continue.");
  }
}

// ===============================
// Loop
// ===============================
void loop()
{
  // Read sensors
  float distance1 = getDistance(TRIG1, ECHO1);

  delay(100);

  float distance2 = getDistance(TRIG2, ECHO2);

  // Determine occupancy
  bool occupied1 = (distance1 <= PARKING_LIMIT);
  bool occupied2 = (distance2 <= PARKING_LIMIT);

  // Calculate slots
  int occupiedSlots = 0;
  int availableSlots = 0;

  if (occupied1)
    occupiedSlots++;
  else
    availableSlots++;

  if (occupied2)
    occupiedSlots++;
  else
    availableSlots++;

  // ===============================
  // Serial Monitor
  // ===============================
  Serial.println();
  Serial.println("===== SMART PARKING =====");

  Serial.print("Slot 1: ");
  Serial.print(occupied1 ? "OCCUPIED" : "AVAILABLE");
  Serial.print(" | Distance: ");
  Serial.print(distance1);
  Serial.println(" cm");

  Serial.print("Slot 2: ");
  Serial.print(occupied2 ? "OCCUPIED" : "AVAILABLE");
  Serial.print(" | Distance: ");
  Serial.print(distance2);
  Serial.println(" cm");

  Serial.print("Available Slots: ");
  Serial.println(availableSlots);

  Serial.print("Occupied Slots: ");
  Serial.println(occupiedSlots);

  Serial.println("=========================");

  // ===============================
  // ThingSpeak
  // ===============================
  if (millis() - lastThingSpeakUpdate >= thingSpeakInterval)
  {
    sendToThingSpeak(
      distance1,
      distance2,
      availableSlots,
      occupiedSlots
    );

    lastThingSpeakUpdate = millis();
  }

  delay(1000);
}
