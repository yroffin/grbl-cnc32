#include "wifi-ctrl.hpp"
#include "webserver-ctrl.hpp"
#include "i18n-ctrl.hpp"
#include "grbl-ctrl.hpp"
#include "storage-ctrl.hpp"
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
WebServerCtrl server;

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

void Simulate()
{
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    char buffer[4096];
    StaticJsonDocument<512> doc;
    switch (server.method())
    {
    case HTTP_GET:
    {
        JsonObject store = doc.createNestedObject("stored");
        JsonObject mpos = store.createNestedObject("mpos");
        float x, y, z;
        bool metric, abs;
        GrblCtrl::instance()->getStoredMpos(&x, &y, &z);
        mpos["x"] = x;
        mpos["y"] = y;
        mpos["z"] = z;
        JsonObject wpos = store.createNestedObject("wpos");
        GrblCtrl::instance()->getStoredWpos(&x, &y, &z);
        wpos["x"] = x;
        wpos["y"] = y;
        wpos["z"] = z;
        JsonObject smodal = store.createNestedObject("modal");
        GrblCtrl::instance()->getStoredModal(&metric, &abs);
        smodal["metric"] = metric;
        smodal["abs"] = abs;
        JsonObject working = doc.createNestedObject("working");
        JsonObject mposw = working.createNestedObject("mpos");
        GrblCtrl::instance()->getWorkingMpos(&x, &y, &z);
        mposw["x"] = x;
        mposw["y"] = y;
        mposw["z"] = z;
        JsonObject wposw = working.createNestedObject("wpos");
        GrblCtrl::instance()->getWorkingWpos(&x, &y, &z);
        wposw["x"] = x;
        wposw["y"] = y;
        wposw["z"] = z;
        JsonObject wmodal = working.createNestedObject("modal");
        GrblCtrl::instance()->getWorkingModal(&metric, &abs);
        wmodal["metric"] = metric;
        wmodal["abs"] = abs;
        serializeJson(doc, buffer, sizeof(buffer));
        break;
    }
    case HTTP_PUT:
        for (int i = 0; i < server.args(); i++)
        {
            if (strcmp(server.argName(i).c_str(), "plain") == 0)
            {
                strcpy(buffer, server.arg(i).c_str());
                GrblCtrl::instance()->serial(buffer);
                // send body with 204
                server.setContentLength(0);
                server.send(204, "application/json", "");
                return;
            }
        }
        break;
    }
    // send body
    server.setContentLength(strlen(buffer));
    server.send(200, "application/json", buffer);
}

void ApiJson(JsonStore *json, const char *filename, const char *backup)
{
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    char buffer[4096 + 1024];
    switch (server.method())
    {
    case HTTP_GET:
        json->get(buffer, sizeof(buffer));
        break;
    case HTTP_PUT:
        for (int i = 0; i < server.args(); i++)
        {
            if (strcmp(server.argName(i).c_str(), "plain") == 0)
            {
                strcpy(buffer, server.arg(i).c_str());
                json->set(buffer, sizeof(buffer));
                json->save(filename, backup);
            }
        }
        break;
    }
    // send body
    server.setContentLength(strlen(buffer));
    server.send(200, "application/json", buffer);
}

void ApiConfig()
{
    ApiJson(JsonConfigCtrl::instance(), "/config.json", "/config.bak");
}

void ApiI18nEnUS()
{
    ApiJson(I18nCtrl::instance(), "/i18n_enUS.json", "/i18n_enUS.bak");
}

void ApiI18nFrFR()
{
    ApiJson(I18nCtrl::instance(), "/i18n_frFR.json", "/i18n_frFR.bak");
}

void (*resetFunc)(void) = 0;
void Reboot()
{
    resetFunc();
}

void Print()
{
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    char buffer[4096];
    StaticJsonDocument<256> doc;
    switch (server.method())
    {
    case HTTP_POST:
        for (int i = 0; i < server.args(); i++)
        {
            if (strcmp(server.argName(i).c_str(), "plain") == 0)
            {
                strcpy(buffer, server.arg(i).c_str());
                deserializeJson(doc, buffer);
                if (doc.containsKey("command"))
                {
                    const char *command = doc["command"];
                    // switch to command mode
                    StorageCtrl::instance()->commands();
                    GrblCtrl::instance()->print(command);
                }
                if (doc.containsKey("file"))
                {
                    const char *file = doc["file"];
                    // switch to file mode
                    StorageCtrl::instance()->files();
                    GrblCtrl::instance()->print(file);
                }
            }
        }
        break;
    }
    // send body
    server.setContentLength(strlen(buffer));
    server.send(200, "application/json", buffer);
}

void Command()
{
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    char buffer[4096];
    StaticJsonDocument<256> doc;
    switch (server.method())
    {
    case HTTP_POST:
        for (int i = 0; i < server.args(); i++)
        {
            if (strcmp(server.argName(i).c_str(), "plain") == 0)
            {
                strcpy(buffer, server.arg(i).c_str());
                deserializeJson(doc, buffer);
                if (doc.containsKey("tryWrite"))
                {
                    const char *command = doc["tryWrite"];
                    bool result = GrblCtrl::instance()->tryWrite(true, "%s\n", command);
                    if (result)
                    {
                        // send body
                        server.setContentLength(0);
                        server.send(204, "application/json", "\0");
                        return;
                    }
                    else
                    {
                        // send body with bad request
                        server.setContentLength(strlen("{}"));
                        server.send(400, "application/json", "{}");
                        return;
                    }
                }
                if (doc.containsKey("forceWrite"))
                {
                    const char *command = doc["forceWrite"];
                    GrblCtrl::instance()->forceWrite(true, "%s\n", command);
                    // send body
                    server.setContentLength(0);
                    server.send(204, "application/json", "\0");
                    return;
                }
                if (doc.containsKey("reset"))
                {
                    const char *command = doc["reset"];
                    GrblCtrl::instance()->reset();
                    // send body
                    server.setContentLength(0);
                    server.send(204, "application/json", "\0");
                    return;
                }
            }
        }
        break;
    }
    // send body
    server.setContentLength(0);
    server.send(204, "application/json", "\0");
}

void Delete()
{
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    char buffer[4096];
    StaticJsonDocument<256> doc;
    switch (server.method())
    {
    case HTTP_POST:
        for (int i = 0; i < server.args(); i++)
        {
            if (strcmp(server.argName(i).c_str(), "plain") == 0)
            {
                strcpy(buffer, server.arg(i).c_str());
                deserializeJson(doc, buffer);
                if (doc.containsKey("file"))
                {
                    const char *file = doc["file"];
                    GrblCtrl::instance()->remove(file);
                }
            }
        }
        break;
    }
    // send body
    server.setContentLength(0);
    server.send(204, "application/json", "\0");
}

void Write()
{
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    char buffer[2048];
    StaticJsonDocument<1024> doc;
    StaticJsonDocument<1024> output;
    switch (server.method())
    {
    case HTTP_POST:
        for (int i = 0; i < server.args(); i++)
        {
            if (strcmp(server.argName(i).c_str(), "plain") == 0)
            {
                int mode = 0;
                const char *file;
                strcpy(buffer, server.arg(i).c_str());
                deserializeJson(doc, buffer);
                if (doc.containsKey("op"))
                {
                    const char *op = doc["op"];
                    // read operation (append)
                    if (strcmp(op, "append") == 0)
                    {
                        mode = 1;
                    }
                    if (strcmp(op, "init") == 0)
                    {
                        mode = 0;
                    }
                }
                if (doc.containsKey("file"))
                {
                    file = doc["file"];
                }
                if (doc.containsKey("data"))
                {
                    const char *data = doc["data"];
                    switch (mode)
                    {
                    case 0:
                        output["written"] = StorageCtrl::instance()->touch(file, data);
                        break;
                    case 1:
                        output["written"] = StorageCtrl::instance()->append(file, data);
                        break;
                    }
                }
            }
        }
        serializeJson(output, buffer, sizeof(buffer));
        break;
    }
    // send body
    server.setContentLength(strlen(buffer));
    server.send(200, "application/json", buffer);
}

void Files()
{
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    char buffer[4096];
    StaticJsonDocument<1024> doc;
    switch (server.method())
    {
    case HTTP_GET:
    {
        int count = StorageCtrl::instance()->scan();
        for (int i = 0; i < count; i++)
        {
            const char *filename = StorageCtrl::instance()->path(i);
            if (StorageCtrl::instance()->isFile(i))
            {
                doc.add(filename);
            }
        }
        serializeJson(doc, buffer, sizeof(buffer));
        break;
    }
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
            log_i("%s", I18nCtrl::instance()->translate(I18N_STD, "TRY", ssid));
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
            TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, "FAIL", ssid));
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
        server.on("/test", HomePage);
        server.on("/api/v1/config/config.json", HTTP_ANY, ApiConfig);
        server.on("/api/v1/i18n/i18n_enUS.json", HTTP_ANY, ApiI18nEnUS);
        server.on("/api/v1/i18n/i18n_frFR.json", HTTP_ANY, ApiI18nFrFR);
        server.on("/api/v1/simulate", HTTP_ANY, Simulate);
        server.on("/api/v1/command", HTTP_ANY, Command);
        server.on("/api/v1/reboot", Reboot);
        server.on("/api/v1/print", Print);
        server.on("/api/v1/delete", Delete);
        server.on("/api/v1/files", Files);
        server.on("/api/v1/file", Write);
        server.mount("/ui", "/static", "");
        server.begin();
        TFT_Screen::instance()->outputConsole(I18nCtrl::instance()->translate(I18N_STD, "SRV", 80));
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
