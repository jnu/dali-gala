#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <FastLED.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Configuration
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
#define LED_PIN 48  // Built-in LED on ESP32-S3-Pico
#define NUM_LEDS 1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

// Global objects
AsyncWebServer server(80);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
CRGB leds[NUM_LEDS];

// HTML page for web interface
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>ESP32-S3-Pico Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html { font-family: Arial; display: inline-block; margin: 0px auto; text-align: center; }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    body { max-width: 600px; margin:0px auto; padding-bottom: 25px; }
    .button { background-color: #4CAF50; border: none; color: white; padding: 15px 32px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer; }
    .button2 { background-color: #555555; }
  </style>
</head>
<body>
  <h2>ESP32-S3-Pico Control Panel</h2>
  <p><span id="uptime">%UPTIME%</span></p>
  <p><span id="freemem">%FREEMEM%</span></p>
  <p><span id="wifi">%WIFI%</span></p>
  <p><button class="button" onclick="toggleLED()">Toggle LED</button></p>
  <p><button class="button button2" onclick="restart()">Restart</button></p>
  <script>
    function toggleLED() {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/toggle", true);
      xhr.send();
    }
    function restart() {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/restart", true);
      xhr.send();
    }
    setInterval(function() {
      getData();
    }, 1000);
    function getData() {
      var xhr = new XMLHttpRequest();
      xhr.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          var data = JSON.parse(this.responseText);
          document.getElementById("uptime").innerHTML = "Uptime: " + data.uptime;
          document.getElementById("freemem").innerHTML = "Free Memory: " + data.freemem + " bytes";
          document.getElementById("wifi").innerHTML = "WiFi RSSI: " + data.wifi + " dBm";
        }
      };
      xhr.open("GET", "/data", true);
      xhr.send();
    }
  </script>
</body>
</html>
)rawliteral";

// Function prototypes
void setupWiFi();
void setupWebServer();

String getUptime();
String getFreeMemory();
String getWiFiRSSI();

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32-S3-Pico Starting...");
  
  // Initialize LED
  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(50);
  leds[0] = CRGB::Red;
  FastLED.show();

  // Setup WiFi
  setupWiFi();
  
  // Setup web server
  setupWebServer();
  
  // Initialize NTP
  timeClient.begin();
  timeClient.setTimeOffset(0); // Adjust for your timezone
  
  // Start server
  server.begin();
  
  Serial.println("Setup complete!");
  leds[0] = CRGB::Green;
  FastLED.show();
}

void loop() {
  // Update NTP time
  timeClient.update();
  
  // Handle WiFi reconnection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    setupWiFi();
  }
  
  delay(100);
}

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("WiFi connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void setupWebServer() {
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  
  // Route for /toggle endpoint
  server.on("/toggle", HTTP_GET, [](AsyncWebServerRequest *request){
    static bool ledState = false;
    ledState = !ledState;
    if (ledState) {
      leds[0] = CRGB::Blue;
    } else {
      leds[0] = CRGB::Green;
    }
    FastLED.show();
    request->send(200, "text/plain", ledState ? "LED ON" : "LED OFF");
  });
  
  // Route for /restart endpoint
  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Restarting...");
    delay(1000);
    ESP.restart();
  });
  
  // Route for /data endpoint
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "{";
    json += "\"uptime\":\"" + getUptime() + "\",";
    json += "\"freemem\":\"" + getFreeMemory() + "\",";
    json += "\"wifi\":\"" + getWiFiRSSI() + "\"";
    json += "}";
    request->send(200, "application/json", json);
  });
}

// Template processor for HTML
String processor(const String& var) {
  if(var == "UPTIME"){
    return getUptime();
  }
  else if(var == "FREEMEM"){
    return getFreeMemory();
  }
  else if(var == "WIFI"){
    return getWiFiRSSI();
  }
  return String();
}

String getUptime() {
  unsigned long uptime = millis() / 1000;
  unsigned long days = uptime / 86400;
  uptime %= 86400;
  unsigned long hours = uptime / 3600;
  uptime %= 3600;
  unsigned long minutes = uptime / 60;
  unsigned long seconds = uptime % 60;
  
  String uptimeStr = "";
  if (days > 0) uptimeStr += String(days) + "d ";
  if (hours > 0) uptimeStr += String(hours) + "h ";
  if (minutes > 0) uptimeStr += String(minutes) + "m ";
  uptimeStr += String(seconds) + "s";
  
  return uptimeStr;
}

String getFreeMemory() {
  return String(ESP.getFreeHeap());
}

String getWiFiRSSI() {
  return String(WiFi.RSSI());
}

