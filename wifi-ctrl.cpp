#include "wifi-ctrl.hpp"
#include "iniFile.h"

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
    char ssid[32];
    *ssid = 0;
    char pass[32];
    *pass = 0;

    if (!ini.validate())
    {
        log_e("Unable to validate /config.ini");
    }
    else
    {
        if (ini.getValue((const char *)"wifi", (const char *)"ssid", (char *)ssid, 32))
        {
            if (ini.getValue((const char *)"wifi", (const char *)"pass", (char *)pass, 32))
            {
                log_i("Wifi controller begin with '%s'", ssid);
                WiFi.begin(ssid, pass);
                uint8_t initWifiCnt = 40;
                log_i("Wifi controller status ...");
                while (WiFi.status() != WL_CONNECTED)
                { // Wait for the Wi-Fi to connect; max 40 retries
                    delay(250);
                    initWifiCnt--;
                    if (initWifiCnt == 0)
                    {
                        TFT_Screen::instance()->outputConsole("Access to %s KO !!!", ssid);
                        delay(2000);
                        break;
                    }
                }
                if (WiFi.status() == WL_CONNECTED)
                {
                    char ip[32];
                    strcpy(ip, WiFi.localIP().toString().c_str());
                    log_i("Wifi IP is %s", ip);
                }
                log_i("Wifi controller status ok");
                WiFi.setSleep(false);
                server.on("/", HomePage);
                log_i("Serving data ...");
                server.begin();
                log_i("Serving data on %d port ...", 80);
            }
            else
            {
                TFT_Screen::instance()->outputConsole("Unable to read wifi password");
            }
        }
        else
        {
            TFT_Screen::instance()->outputConsole("Unable to read wifi ssid");
        }
    }
}

void WifiCtrl::serve()
{
    server.handleClient(); // Listen for client connections
}