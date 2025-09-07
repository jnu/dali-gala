#include <Arduino.h>

// Basic test sketch for ESP32-S3-Pico
// This sketch tests basic functionality without external components

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("=== ESP32-S3-Pico Basic Test ===");
  Serial.println("Testing basic functionality...");
  
  // Test built-in LED
  pinMode(48, OUTPUT);
  Serial.println("Testing built-in LED...");
  
  for (int i = 0; i < 5; i++) {
    digitalWrite(48, HIGH);
    delay(200);
    digitalWrite(48, LOW);
    delay(200);
  }
  
  // Test WiFi
  Serial.println("Testing WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);
  
  // Test I2C
  Serial.println("Testing I2C...");
  Wire.begin(8, 9);  // SDA=8, SCL=9
  Wire.beginTransmission(0x3C);
  byte error = Wire.endTransmission();
  if (error == 0) {
    Serial.println("I2C device found at address 0x3C");
  } else {
    Serial.println("No I2C device at address 0x3C");
  }
  
  // Test SPI
  Serial.println("Testing SPI...");
  SPI.begin(12, 13, 11, 10);  // SCK, MISO, MOSI, SS
  
  // Test ADC
  Serial.println("Testing ADC...");
  int adcValue = analogRead(1);  // GPIO1
  Serial.printf("ADC value on GPIO1: %d\n", adcValue);
  
  // Test PWM
  Serial.println("Testing PWM...");
  ledcSetup(0, 5000, 8);  // Channel 0, 5kHz, 8-bit resolution
  ledcAttachPin(48, 0);   // Attach LED pin to channel 0
  
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    ledcWrite(0, dutyCycle);
    delay(10);
  }
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    ledcWrite(0, dutyCycle);
    delay(10);
  }
  
  Serial.println("Basic test complete!");
}

void loop() {
  // Blink LED every second
  digitalWrite(48, HIGH);
  delay(500);
  digitalWrite(48, LOW);
  delay(500);
  
  // Print system info every 10 seconds
  static unsigned long lastInfo = 0;
  if (millis() - lastInfo >= 10000) {
    Serial.printf("Uptime: %lu seconds, Free heap: %lu bytes\n", 
                  millis() / 1000, ESP.getFreeHeap());
    lastInfo = millis();
  }
}

