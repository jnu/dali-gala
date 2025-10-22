#include "GalaWebServer.h"
#include "esp_log.h"

static const char *TAG = "GalaWebServer";

static AsyncWebServer server(CONFIG_WEB_SERVER_PORT);


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
  bool broadcast = false;
  bool levelSet = false;
  uint addr = 0xFF;
  uint level = 0;
  
  // Check if `addr` is provided in the request.
  if(json.as<JsonObject>().containsKey("addr")) {
    addr = json.as<JsonObject>()["addr"];
    broadcast = false;
  }

  if(json.as<JsonObject>().containsKey("level")) {
    level = json.as<JsonObject>()["level"];
    levelSet = true;
  }

  if (broadcast) {
    ESP_LOGI(TAG, "Setting all lamps to: %d\n", state);
    if (state) {
      GalaDALIAllOn();
    } else {
      GalaDALIAllOff();
    }
    // TODO handle level
  } else {
    ESP_LOGI(TAG, "Setting lamp %d to: %d\n", addr, level);
    if (!levelSet) {
      if (state) {
        level = 254;
      } else {
        level = 0;
      }
    }
    GalaDALISetLevel(addr, level);
  }

  response->print("{\"success\": true}");
  request->send(response);
}

void handleCommission(AsyncWebServerRequest *request)
{
  GalaDALICommission();
  request->send(200, "application/json", "{\"success\": true}");
}

void handleSetCCT(AsyncWebServerRequest *request, JsonVariant &json)
{
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  uint8_t addr = json.as<JsonObject>()["addr"];
  uint16_t value = json.as<JsonObject>()["value"];
  uint8_t result = GalaDALISetTemp(addr, value);
  if (result != 0) {
    sendError(request, 500, "Failed to set CCT");
    return;
  }
  response->print("{\"success\": true}");
  request->send(response);
}

void handleCmd(AsyncWebServerRequest *request, JsonVariant &json)
{
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  uint16_t cmd = json.as<JsonObject>()["cmd"];
  uint8_t arg = json.as<JsonObject>()["arg"];
  int16_t result = GalaDALICmd(cmd, arg);
  response->print("{\"success\": true, \"result\": " + String(result) + "}");
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

      AsyncCallbackJsonWebHandler* setCCTHandler = new AsyncCallbackJsonWebHandler("/api/v1/cct", handleSetCCT);
      setCCTHandler->setMethod(HTTP_POST);
      setCCTHandler->setMaxContentLength(1024);
      server.addHandler(setCCTHandler);

      AsyncCallbackJsonWebHandler* cmdHandler = new AsyncCallbackJsonWebHandler("/api/v1/cmd", handleCmd);
      cmdHandler->setMethod(HTTP_POST);
      cmdHandler->setMaxContentLength(1024);
      server.addHandler(cmdHandler);

      server.on(
        "^\\/api\\/v1\\/commission$",
        HTTP_POST,
        handleCommission
      );
    
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