#include "WS_WIFI.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "webapp.h"
#include <stdarg.h>


// Configuration for the Wifi Access Point.
// Users can connect using this SSID, password, on this IP.
const char *ssid = APSSID;                
const char *password = APPSK;               
IPAddress apIP(10, 10, 0, 1);

char ipStr[16];
static AsyncWebServer server(80);

#ifdef __cplusplus
extern "C" {
#endif


void handleRoot(AsyncWebServerRequest *request)
{
  printf("Home page\n");
  request->send(200, "text/html", APP_HTML_CONTENT);
}


void sendError(AsyncWebServerRequest *request,int code, String message)
{
  // TODO: better serialization. Can't handle quotes right now.
  String body = "{\"error\": \"" + message + "\", \"code\": " + code + "}";
  request->send(code, "application/json", body);
}

void handleAddressesQuery(AsyncWebServerRequest *request)
{
  // Return a list of addresses
  String body = "{\"addresses\": [";
  for (int i = 0; i < DALI_NUM; i++) {
    if (i > 0) {
      body += ",";
    }
    body += String(DALI_Addr[i]);
  }
  body += "]}";
  request->send(200, "application/json", body);
}

void handleAddressQuery(AsyncWebServerRequest *request)
{
  String address = request->pathArg(0);
  uint addr = address.toInt();
  
  int16_t state = DALICheckStatus(addr);
  String body = "{\"status\":" + String(state) + "}";
  request->send(200, "application/json", body);
}

/**
 * Handle lights request.
 */
void handleLights(AsyncWebServerRequest *request, JsonVariant &json)
{
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  bool state = json.as<JsonObject>()["state"];

  printf("Lights request: %d\n", state);
  // TODO - check if state is valid

  // Set lights state
  if(state) {
    Lighten_ALL();
  } else {
    Extinguish_ALL();
  }

  response->print("{\"success\": true}");
  request->send(response);
}


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
  printf("AP IP address: ");
  sprintf(ipStr, "%d.%d.%d.%d", myIP[0], myIP[1], myIP[2], myIP[3]);
  printf("%s\r\n", ipStr);

  printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA:%d\r\n", DALI_NUM);
  
  server.on(
    "^\\/$",
    HTTP_GET,
    handleRoot
  );
  server.on(
    "^\\/api\\/v1\\/addresses$",
    HTTP_GET,
    handleAddressesQuery
  );
  server.on(
    "^\\/api\\/v1\\/addresses\\/([0-9]+)$",
    HTTP_GET,
    handleAddressQuery
  );

  AsyncCallbackJsonWebHandler* apiV1LightsHandler = new AsyncCallbackJsonWebHandler("^\\/api\\/v1\\/addresses$", handleLights);
  apiV1LightsHandler->setMethod(HTTP_POST);
  server.addHandler(apiV1LightsHandler);

  server.begin();

  printf("Web server started\n");
}

#ifdef __cplusplus
}
#endif














