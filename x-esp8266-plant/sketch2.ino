#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>


// Replace with your network credentials
const char* ssid     = "EggFooYoung-2.4";
const char* password = "furongdan";


// Replace with your IFTTT Event Name and API Key
const char* eventName = "plant_water";
const char* apiKey    = "bQf5f96-yUi2cS4wqzzyKO";

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  // Trigger the email
 sendEmailWebhook("Hello from Arduino!");
}

void loop() {
  // Keep empty or add logic to trigger emails based on sensors
}


void sendEmailWebhook(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    // Construct the URL
    String url = "http://maker.ifttt.com/trigger/" + String(eventName) + "/json/with/key/" + String(apiKey);
    
    // Optional: Send data as JSON (Value1)
    String jsonPayload = "{\"value1\":\"" + message + "\"}";

    http.begin(client, url);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      Serial.println("Email sent successfully!");
    } else {
      Serial.print("Error sending email: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
}