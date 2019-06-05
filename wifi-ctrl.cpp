#include "wifi-ctrl.hpp"
#include "i18n-ctrl.hpp"
#include "json-config.hpp"
#include "utils.hpp"

// Storage controller
WifiCtrl *__instance_wifi = 0;

// singleton
WifiCtrl *WifiCtrl::instance()
{
    if (__instance_wifi == 0)
    {
        __instance_wifi = new WifiCtrl();
    }
    return __instance_wifi;
}

WifiCtrl::WifiCtrl()
{
}

// webserver
WebServer server(80);

void HomePage()
{
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
    server.sendContent("");
    server.client().stop(); // Stop is needed because no content length was sent
}

void ApiConfig()
{
    JsonConfigCtrl *jsonConfig = JsonConfigCtrl::instance();
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    char buffer[2048];
    switch (server.method())
    {
    case HTTP_GET:
        jsonConfig->get(buffer, 2048);
        break;
    case HTTP_PUT:
        for (int i = 0; i < server.args(); i++)
        {
            if (strcmp(server.argName(i).c_str(), "plain") == 0)
            {
                strcpy(buffer, server.arg(i).c_str());
                jsonConfig->set(buffer, 2048);
                jsonConfig->save("/config.json", "/config.bak");
            }
        }
        break;
    }
    // send body
    server.setContentLength(strlen(buffer));
    server.send(200, "application/json", buffer);
}

// Init phase
void WifiCtrl::setup()
{
    JsonConfigCtrl *jsonConfig = JsonConfigCtrl::instance();

    this->lastConnect = millis();
    if (jsonConfig->getSize("wifi") > 0)
    {
        this->phase = wifiNext;
    }
}

// Init wifi and serve at the end
void WifiCtrl::loop()
{
    JsonConfigCtrl *jsonConfig = JsonConfigCtrl::instance();

    switch (this->phase)
    {
    case wifiNext:
        if (jsonConfig->getSize("wifi") > 0)
        {
            const char *ssid = jsonConfig->getAsString("wifi", current, "ssid", "");
            const char *pass = jsonConfig->getAsString("wifi", current, "pass", "");
            log_i("%s", I18nCtrl::instance()->translate(I18N_STD, I18N_WIFI_TRYING, ssid));
            WiFi.begin(ssid, pass);
            TFT_Screen::instance()->notifyWifiStatus(ssid);
            this->phase = wifiInit;
        }
        break;
    case wifiInit:
        if (jsonConfig->getSize("wifi") > 0)
        {
            const char *ssid = jsonConfig->getAsString("wifi", current, "ssid", "");
            const char *pass = jsonConfig->getAsString("wifi", current, "pass", "");
            if (millis() - this->lastConnect < 1000)
            {
                return;
            }
            this->lastConnect = millis();
            if (WiFi.status() == WL_CONNECTED)
            {
                TFT_Screen::instance()->notifyWifiStatus(WiFi.localIP().toString().c_str());
                this->phase = wifiConnected;
                return;
            }
            // stay on current with some retry
            if (this->retry < 5)
            {
                TFT_Screen::instance()->notifyWifiStatus("retry");
                this->retry++;
                return;
            }
            TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, I18N_WIFI_KO, ssid));
            this->retry = 0;
            this->current++;
            if (this->current >= jsonConfig->getSize("wifi"))
            {
                this->current = 0;
            }
            this->phase = wifiNext;
            TFT_Screen::instance()->notifyWifiStatus("KO");
        }
        break;
    case wifiConnected:
        WiFi.setSleep(false);
        server.on("/", HomePage);
        server.on("/api/v1/config/config.json", HTTP_ANY, ApiConfig);
        server.begin();
        TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, I18N_WIFI_SERVE, 80));
        this->phase = unixTime;
        break;
    case unixTime:
    {
        time_t unixTime = Utils::fixTime();
        struct tm *lt = localtime(&unixTime);
        char str[32];
        strftime(str, sizeof str, "%d/%m/%Y %H:%M:%S", lt);
        log_i("Local time %s", str);
    }
        this->phase = serveData;
        break;
    case serveData:
        server.handleClient(); // Listen for client connections
        break;
    }
}
