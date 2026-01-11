#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT11.h>
#include <time.h>


// Replace with your network credentials
const char *ssid = "EggFooYoung-2.4";
const char *password = "furongdan";

// Replace with your IFTTT Event Name and API Key
const char *eventName = "plant_water";
const char *apiKey = "bQf5f96-yUi2cS4wqzzyKO";

// DHT11
DHT11 dht11(2); // GPIO2
int gTemperature = 0;
int gHumidity = 0;
int gLoopTimes = 0;

// sleep
const uint64_t sleepDuration = 10E6; // 10 seconds (adjust as needed)

void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 30)
    {
        delay(500);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println("\nWiFi connected");

        // Configure time with NTP server
        configTime(-8 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    }
    else
    {
        Serial.println("\nWiFi connection failed!");
    }
}

void setup()
{
    Serial.begin(9600);
    pinMode(2, OUTPUT); // LED on GPIO2

    // print set up
    Serial.println("Beginning ESP8266 + DHT11 + Webhooks ...");

    // Go to deep sleep
    Serial.flush(); // Ensure all serial data is sent
}

void loop()
{   
    //Turn on LED to indicate activity
    digitalWrite(2, LOW); // turn on LED
    delay(1000);
    digitalWrite(2, HIGH); // turn off LED
    delay(3000);

    // Connect to WiFi
    if (gLoopTimes % 10 == 0){
        initWiFi();
    }
    // Read DHT11 sensor
    delay(2000); // Give sensor time to stabilize

    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);

    int hour = timeinfo->tm_hour;
  
    Serial.print("Current hour: ");
    Serial.println(hour);

    int min = timeinfo->tm_min;
    Serial.print("Current minute: ");
    Serial.println(min);

    Serial.flush(); // Ensure all serial data is sent

    if (WiFi.status() == WL_CONNECTED)
    {
        int temperature = 0, humidity = 0;
        int result = dht11.readTemperatureHumidity(temperature, humidity);

        if (result == 0)
        {
            Serial.print("Temperature: ");
            Serial.print(temperature);
            Serial.print(" °C\tHumidity: ");
            Serial.print(humidity);
            Serial.println(" %");

            // Send email with sensor data
            // sendEmailWebhook(String(temperature), String(humidity));
            // Serial.println("Data sent successfully!");
        }
        else
        {
            Serial.print("Sensor error: ");
            Serial.println(DHT11::getErrorString(result));

            // Optionally send error notification
            // sendEmailWebhook("Error", DHT11::getErrorString(result));
        }
    }

    // Wait a bit for serial output to complete
    delay(1000);
    Serial.flush(); // Ensure all serial data is sent

    // Enter light sleep
    Serial.println("Entering Light Sleep...");
    WiFi.mode(WIFI_OFF);   // you must turn the modem off; using disconnect won't work
    readVoltage();         // read internal VCC
    printMillis();         // show millis() across sleep, including Serial.flush()
    digitalWrite(2, HIGH); // turn the LED off so they know the CPU isn't running

    wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
    // wifi_fpm_set_wakeup_cb(wakeupCallback);
    wifi_fpm_open();
    wifi_fpm_do_sleep(sleepDuration); // Sleep for defined duration
    delay(sleepDuration / 1000 + 1);  // delay needs to be longer than sleep duration
    Serial.println(F("Woke up!"));    // the interrupt callback hits before this is executed

    // Increment loop counter
    gLoopTimes = (gLoopTimes + 1) % 10000;
}

void readVoltage()
{ // read internal VCC
    float volts = ESP.getVcc();
    Serial.printf("The internal VCC reads %1.2f volts\n", volts / 1000);
}

void printMillis()
{
    Serial.print(F("millis() = ")); // show that millis() isn't correct across most Sleep modes
    Serial.println(millis());
    Serial.flush(); // needs a Serial.flush() else it may not print the whole message before sleeping
}

void sendEmailWebhook(String temp, String humid)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        WiFiClient client;
        HTTPClient http;

        // Construct the URL
        String url = "http://maker.ifttt.com/trigger/" + String(eventName) + "/json/with/key/" + String(apiKey);

        // Create JSON payload with temp and humid
        String jsonPayload = "{\"Temperature\":\"" + temp + "\",\"Humidity\":\"" + humid + "\"}";

        http.begin(client, url);
        http.addHeader("Content-Type", "application/json");

        int httpResponseCode = http.POST(jsonPayload);

        if (httpResponseCode > 0)
        {
            Serial.println("Email sent successfully!");
        }
        else
        {
            Serial.print("Error sending email: ");
            Serial.println(httpResponseCode);
        }
        http.end();
    }
}