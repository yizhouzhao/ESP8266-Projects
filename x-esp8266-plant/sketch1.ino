#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT11.h>

const char* ssid = "ESP8266-Hello";
const char* password = "helloworld";

ESP8266WebServer server(80);
DHT11 dht11(2); // GPIO2

// Make these global so handleRoot() can use them
int gTemperature = 0;
int gHumidity = 0;
int gLastReadResult = -1;
unsigned long gLastReadMs = 0;

void handleRoot();

void setup() {
  Serial.begin(9600);
  Serial.println();

  WiFi.mode(WIFI_AP);
  bool ok = WiFi.softAP(ssid, password);
  Serial.print("SoftAP start: ");
  Serial.println(ok ? "OK" : "FAILED");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.onNotFound([]() {
    server.send(404, "text/plain", "Not found");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // DHT11 should not be polled too fast (about 1s+)
  if (millis() - gLastReadMs >= 2000) {
    gLastReadMs = millis();

    int t = 0, h = 0;
    int result = dht11.readTemperatureHumidity(t, h);
    gLastReadResult = result;

    if (result == 0) {
      gTemperature = t;
      gHumidity = h;

      Serial.print("Temperature: ");
      Serial.print(gTemperature);
      Serial.print(" °C\tHumidity: ");
      Serial.print(gHumidity);
      Serial.println(" %");
    } else {
      Serial.println(DHT11::getErrorString(result));
    }
  }

  server.handleClient();
}

void handleRoot() {
  char buffer[700];

  if (gLastReadResult == 0) {
    // Use %d for integers
    snprintf(buffer, sizeof(buffer),
      "<!DOCTYPE html>"
      "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'>"
      "<style>"
      "body{font-family:Arial;text-align:center;margin-top:30px;}"
      ".box{width:340px;margin:12px auto;padding:18px;border-radius:16px;font-size:34px;color:white;font-weight:bold;}"
      "</style></head><body>"
      "<div class='box' style='background:#2d89ef;'>Humidity: %d %%</div>"
      "<div class='box' style='background:#00a300;'>Temperature: %d &deg;C</div>"
      "</body></html>",
      gHumidity, gTemperature
    );
  } else {
    snprintf(buffer, sizeof(buffer),
      "<!DOCTYPE html>"
      "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'></head><body>"
      "<h2>Sensor read failed</h2>"
      "<p>%s</p>"
      "</body></html>",
      DHT11::getErrorString(gLastReadResult)
    );
  }

  server.send(200, "text/html", buffer);
}
