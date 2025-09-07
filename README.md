# ESP32-S3-Pico Project

A comprehensive ESP32-S3-Pico project featuring WiFi connectivity, web server, OLED display, LED control, and various sensors.

## Features

- **WiFi Connectivity**: Connects to your WiFi network and provides a web interface
- **Web Server**: Built-in web interface for monitoring and control
- **OLED Display**: 128x64 OLED display showing system information
- **LED Control**: RGB LED control via web interface
- **NTP Time**: Network time synchronization
- **Real-time Monitoring**: System uptime, memory usage, and WiFi signal strength
- **Responsive Web UI**: Mobile-friendly web interface

## Hardware Requirements

- ESP32-S3-Pico development board
- 128x64 OLED display (I2C, SSD1306)
- WS2812 RGB LED (optional, can use built-in LED)
- USB-C cable for programming and power

## Pin Connections

| Component | ESP32-S3-Pico Pin | Description |
|-----------|-------------------|-------------|
| OLED SDA  | GPIO8             | I2C Data    |
| OLED SCL  | GPIO9             | I2C Clock   |
| OLED VCC  | 3.3V              | Power       |
| OLED GND  | GND               | Ground      |
| LED Data  | GPIO48            | Built-in LED |
| USB       | USB-C             | Programming & Power |

## Setup Instructions

### 1. Install PlatformIO

Install PlatformIO IDE or PlatformIO Core:

**VS Code Extension:**
- Install "PlatformIO IDE" extension in VS Code

**Command Line:**
```bash
pip install platformio
```

### 2. Clone and Open Project

```bash
git clone <your-repo-url>
cd dali-gala
```

### 3. Configure WiFi

Edit `src/main.cpp` and update these lines with your WiFi credentials:

```cpp
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"
```

### 4. Build and Upload

**VS Code:**
- Click the PlatformIO icon in the sidebar
- Click "Build" to compile
- Click "Upload" to flash to your ESP32-S3-Pico

**Command Line:**
```bash
# Build the project
pio run

# Upload to device
pio run --target upload

# Monitor serial output
pio device monitor
```

### 5. Access Web Interface

After uploading, the ESP32 will:
1. Connect to WiFi
2. Start the web server
3. Display the IP address on the OLED screen

Open your web browser and navigate to the IP address shown on the OLED display.

## Web Interface Features

- **System Monitor**: Real-time display of uptime, memory usage, and WiFi signal
- **LED Control**: Toggle the built-in LED on/off
- **System Restart**: Restart the ESP32 remotely
- **Auto-refresh**: Data updates every second

## Project Structure

```
dali-gala/
├── platformio.ini          # PlatformIO configuration
├── src/
│   └── main.cpp            # Main Arduino sketch
├── lib/                     # Custom libraries (if any)
├── include/                 # Header files (if any)
└── README.md               # This file
```

## Configuration Options

### Build Environments

- **esp32-s3-pico**: Standard build with optimized settings
- **esp32-s3-pico-debug**: Debug build with verbose logging

### Customization

You can modify the following in `src/main.cpp`:

- **Display Settings**: Change OLED dimensions and I2C address
- **LED Configuration**: Modify LED pin and behavior
- **Web Server Port**: Change from default port 80
- **Update Intervals**: Adjust display and data update frequencies

## Troubleshooting

### Common Issues

1. **WiFi Connection Failed**
   - Check SSID and password
   - Ensure 2.4GHz WiFi (ESP32-S3 doesn't support 5GHz)
   - Check WiFi signal strength

2. **OLED Display Not Working**
   - Verify I2C connections (SDA/SCL)
   - Check I2C address (default: 0x3C)
   - Ensure proper power supply

3. **Upload Failed**
   - Check USB cable connection
   - Press and hold BOOT button during upload
   - Verify board selection in platformio.ini

4. **Web Interface Not Accessible**
   - Check serial monitor for IP address
   - Ensure device is on the same network
   - Check firewall settings

### Serial Monitor Output

The ESP32 outputs debug information to the serial monitor at 115200 baud. Use this to troubleshoot connection issues.

## Advanced Features

### Adding Sensors

The project includes libraries for common sensors:

- **DHT11/DHT22**: Temperature and humidity
- **FastLED**: Advanced LED control
- **ArduinoJson**: JSON parsing and generation
- **ESPAsyncWebServer**: Asynchronous web server

### Custom Endpoints

Add new web endpoints in the `setupWebServer()` function:

```cpp
server.on("/custom", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Custom response");
});
```

### OTA Updates

Enable Over-The-Air updates by adding the ArduinoOTA library and implementing update functionality.

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is open source. Feel free to modify and distribute.

## Support

For issues and questions:
- Check the troubleshooting section
- Review the serial monitor output
- Consult ESP32-S3 documentation
- Check PlatformIO documentation

## Resources

- [ESP32-S3 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s3_datasheet_en.pdf)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [Arduino ESP32 Reference](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
- [FastLED Library](https://fastled.io/)
- [Adafruit GFX Library](https://github.com/adafruit/Adafruit-GFX-Library)

