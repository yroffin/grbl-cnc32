#include "webserver-ctrl.hpp"
#include "i18n-ctrl.hpp"

WebServerCtrl::WebServerCtrl()
    : WebServer(80)
{
}

void WebServerCtrl::mount(const char *uri, const char *path, const char *cache_header)
{
    TFT_Screen::instance()->outputConsole("WebServerCtrl setup ...");
    this->addHandler(new StaticCustomRequestHandler(path, uri, cache_header));
}

void WebServerCtrl::handleClient()
{
    if (_currentStatus == HC_NONE)
    {
        WiFiClient client = _server.available();
        if (!client)
        {
            return;
        }

        log_v("New client");

        _currentClient = client;
        _currentStatus = HC_WAIT_READ;
        _statusChange = millis();
    }

    bool keepCurrentClient = false;
    bool callYield = false;

    if (_currentClient.connected())
    {
        switch (_currentStatus)
        {
        case HC_NONE:
            // No-op to avoid C++ compiler warning
            break;
        case HC_WAIT_READ:
            // Wait for data from client to become available
            if (_currentClient.available())
            {
                if (_parseRequest(_currentClient))
                {
                    _currentClient.setTimeout(HTTP_MAX_SEND_WAIT);
                    _contentLength = CONTENT_LENGTH_NOT_SET;
                    _handleRequest();

                    _currentClient.stop();

                    if (_currentClient.connected())
                    {
                        _currentStatus = HC_WAIT_CLOSE;
                        _statusChange = millis();
                        keepCurrentClient = true;
                    }
                }
            }
            else
            { // !_currentClient.available()
                if (millis() - _statusChange <= HTTP_MAX_DATA_WAIT)
                {
                    keepCurrentClient = true;
                }
                callYield = true;
            }
            break;
        case HC_WAIT_CLOSE:
            // Wait for client to close the connection
            if (millis() - _statusChange <= HTTP_MAX_CLOSE_WAIT)
            {
                keepCurrentClient = true;
                callYield = true;
            }
        }
    }

    if (!keepCurrentClient)
    {
        _currentClient = WiFiClient();
        _currentStatus = HC_NONE;
        _currentUpload.reset();
    }

    if (callYield)
    {
        yield();
    }
}
