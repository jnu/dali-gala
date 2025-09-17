#include "GalaWebServer.h"
#include "esp_log.h"

static const char *TAG = "GalaWebServer";

static AsyncWebServer server(WEB_SERVER_PORT);


void sendError(AsyncWebServerRequest *request,int code, String message)
{
  // TODO: better serialization. Can't handle quotes right now.
  String body = "{\"error\": \"" + message + "\", \"code\": " + code + "}";
  request->send(code, "application/json", body);
}

void handleAddressesQuery(AsyncWebServerRequest *request)
{
  int16_t* devices = GalaDALIScanAllAddresses();
  // Return a list of all device statuses
  String body = "{\"statuses\": [";
  for (uint8_t i = 0; i < 64; i++) {
    if (i > 0) {
      body += ",";
    }
    body += String(devices[i]);
  }
  body += "]}";
  request->send(200, "application/json", body);
}

void handleAddressQuery(AsyncWebServerRequest *request)
{
  String address = request->pathArg(0);
  uint addr = address.toInt();
  
  int16_t state = GalaDALICheckStatus(addr);
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

  ESP_LOGI(TAG, "Setting all lamps to: %d\n", state);
  // TODO - check if state is valid

  // Set lights state
  if(state) {
    GalaDALIAllOn();
  } else {
    GalaDALIAllOff();
  }

  response->print("{\"success\": true}");
  request->send(response);
}

/**
 * Initialize the web server.
 *
 * This will initialize the REST API and serve static assets from the FS.
 *
 * @return true if successful, false otherwise
 */
bool GalaWebServerInit(void)
{
    // Initialize the webapp
    if (!GalaWebAppInit()) {
        ESP_LOGI(TAG, "Failed to initialize the webapp");
        return false;
    }

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->redirect("/index.html");
      });
      server.serveStatic("/index.html", LittleFS, "/app.html");
    
      AsyncCallbackJsonWebHandler* lightsHandler = new AsyncCallbackJsonWebHandler("/api/v1/devices", handleLights);
      lightsHandler->setMethod(HTTP_POST);
      lightsHandler->setMaxContentLength(1024);
      server.addHandler(lightsHandler);
    
    
      server.on(
        "^\\/api\\/v1\\/devices$",
        HTTP_GET,
        handleAddressesQuery
      );
      server.on(
        "^\\/api\\/v1\\/devices\\/([0-9]+)$",
        HTTP_GET,
        handleAddressQuery
      );
    
      server.begin();
    
      ESP_LOGI(TAG, "Web server started\n");

      return true;
}