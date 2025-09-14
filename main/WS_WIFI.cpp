#include "WS_WIFI.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "app_html.h"
#include <stdarg.h>


// The name and password of the WiFi access point
const char *ssid = APSSID;                
const char *password = APPSK;               
IPAddress apIP(10, 10, 0, 1);    // Set the IP address of the AP

char ipStr[16];
WebServer server(80);                               

uint8_t sliderValues[64] = {0};
bool DALI_Loop = 1;

#ifdef __cplusplus
extern "C" {
#endif


void handleRoot() {
  String myhtmlPage = APP_HTML_CONTENT;

  server.send(200, "text/html", myhtmlPage); 
  printf("The user visited the home page\r\n");
}


void handleSetSlider() {
  if (server.hasArg("sliderId") && server.hasArg("value")) {
    int sliderId = server.arg("sliderId").toInt();
    int value = server.arg("value").toInt();
    
    // 确保 sliderId 在合法范围内
    if (sliderId >= 1 && sliderId <= DALI_NUM) {
      setBrightness(sliderId, value);
    }
  }
  server.send(200, "text/plain", "OK");
}

// 设置亮度的函数（需要实现具体逻辑）
void setBrightness(int sliderId, int value) {
    // 这里根据 sliderId 设置亮度
    Luminaire_Brightness(value, DALI_Addr[sliderId-1]);
    printf("Slider %d value: %d\n", sliderId, value);
}

void handleSwitch(uint8_t ledNumber) {
  switch(ledNumber){
    case 1:
      printf("RGB On.\r\n");
      Lighten_ALL();                                                       
      break;
    case 2:
      printf("RGB Off.\r\n");
      Extinguish_ALL();                                                    
      break;
  }
  server.send(200, "text/plain", "OK");
}
void handleALLOn()     { handleSwitch(1); }
void handleALLOff()    { handleSwitch(2); }
void handleLoop()      { DALI_Loop = ! DALI_Loop;}


void WIFI_Init()
{
  // Initialize NVS first (required for WiFi)
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
  
  WiFi.mode(WIFI_AP); 
  esp_wifi_set_ps(WIFI_PS_NONE);
  while(!WiFi.softAP(ssid, password)) {
    printf("Soft AP creation failed.\r\n");
    printf("Try setting up the WIFI again.\r\n");
    printf("Tried to connect to %s with password %s\r\n", ssid, password);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  } 
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); // Set the IP address and gateway of the AP
  
  IPAddress myIP = WiFi.softAPIP();
  uint32_t ipAddress = WiFi.softAPIP();
  printf("AP IP address: ");
  sprintf(ipStr, "%d.%d.%d.%d", myIP[0], myIP[1], myIP[2], myIP[3]);
  printf("%s\r\n", ipStr);

  printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA:%d\r\n", DALI_NUM);
  server.on("/", handleRoot);
  server.on("/SetSlider", handleSetSlider);
  server.on("/ALLOn"       , handleALLOn);
  server.on("/ALLOff"      , handleALLOff);
  server.on("/Loop"        , handleLoop);

  server.begin(); 
  printf("Web server started\r\n");
}

void WIFI_Loop()
{
  server.handleClient(); // Processing requests from clients
}

#ifdef __cplusplus
}
#endif














