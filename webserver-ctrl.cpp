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
