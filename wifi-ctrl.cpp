#include "wifi-ctrl.hpp"
#include "i18n-ctrl.hpp"
#include "iniFile.h"
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

IniFile ini("/config.ini");

void WifiCtrl::init()
{
    if (!ini.validate())
    {
        log_e("Unable to validate /config.ini");
    }
    else
    {
        *this->ssid = 0;
        char pass[32];
        *pass = 0;

        if (ini.getValue((const char *)"wifi", (const char *)"ssid", (char *)ssid, 32))
        {
            if (ini.getValue((const char *)"wifi", (const char *)"pass", (char *)pass, 32))
            {
                this->phase = wifiInit;
                log_i("%s", I18nCtrl::instance()->translate(I18N_STD, I18N_WIFI_TRYING, this->ssid));
                WiFi.begin(ssid, pass);
            }
            else
            {
                log_i("%s", I18nCtrl::instance()->translate(I18N_STD, I18N_WIFI_VALID_PASS));
            }
        }
        else
        {
            log_i("%s", I18nCtrl::instance()->translate(I18N_STD, I18N_WIFI_VALID_SSID));
        }
    }
}

// Init wifi and serve at the end
void WifiCtrl::serve()
{
    switch (this->phase)
    {
    case wifiInit:
        if (millis() - this->lastConnect < 1000)
        {
            return;
        }
        TFT_Screen::instance()->notifyWifiStatus(this->ssid);
        this->lastConnect = millis();
        if (WiFi.status() == WL_CONNECTED)
        {
            TFT_Screen::instance()->notifyWifiStatus(WiFi.localIP().toString().c_str());
            this->phase = wifiConnected;
            return;
        }
        TFT_Screen::instance()->notifyWifiStatus("KO");
        TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, I18N_WIFI_KO, this->ssid));
        break;
    case wifiConnected:
        WiFi.setSleep(false);
        server.on("/", HomePage);
        server.begin();
        TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, I18N_WIFI_SERVE, 80));
        this->phase = serveData;
        break;
    case serveData:
        server.handleClient(); // Listen for client connections
        break;
    }
}