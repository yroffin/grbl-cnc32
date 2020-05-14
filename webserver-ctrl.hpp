#ifndef _WEBSERVER_CONTROLLER_
#define _WEBSERVER_CONTROLLER_

#include "config.h"
#include "ui.hpp"
#include "WebServer.h"
#include "SD.h"

struct Entry
{
  const char endsWith[16];
  const char mimeType[32];
};

// Table of extension->MIME strings stored in PROGMEM, needs to be global due to GCC section typing rules
const Entry mimeTable[] =
    {
        {".html", "text/html"},
        {".htm", "text/html"},
        {".css", "text/css"},
        {".txt", "text/plain"},
        {".js", "application/javascript"},
        {".json", "application/json"},
        {".png", "image/png"},
        {".gif", "image/gif"},
        {".jpg", "image/jpeg"},
        {".ico", "image/x-icon"},
        {".svg", "image/svg+xml"},
        {".ttf", "application/x-font-ttf"},
        {".otf", "application/x-font-opentype"},
        {".woff", "application/font-woff"},
        {".woff2", "application/font-woff2"},
        {".eot", "application/vnd.ms-fontobject"},
        {".sfnt", "application/font-sfnt"},
        {".xml", "text/xml"},
        {".pdf", "application/pdf"},
        {".zip", "application/zip"},
        {".gz", "application/x-gzip"},
        {".appcache", "text/cache-manifest"},
        {"", "application/octet-stream"}};

class WebServerCtrl : public WebServer
{
public:
  WebServerCtrl();
  void mount(const char *uri, const char *path, const char *cache_header);

private:
};

class StaticCustomRequestHandler : public RequestHandler
{
public:
  StaticCustomRequestHandler(const char *path, const char *uri, const char *cache_header)
      : _uri(uri), _path(path), _cache_header(cache_header)
  {
    _isFile = StaticCustomRequestHandler::exists(path);
    TFT_Screen::instance()->outputConsole("Mount %s for base url %s - %d", path, uri, _isFile);
    _baseUriLength = _uri.length();
  }

  bool canHandle(HTTPMethod requestMethod, String requestUri) override
  {
    if (requestMethod != HTTP_GET)
      return false;

    if (!_isFile || !requestUri.startsWith(_uri))
    {
      return false;
    }

    return true;
  }

  bool handle(WebServer &server, HTTPMethod requestMethod, String requestUri) override
  {
    if (!canHandle(requestMethod, requestUri))
      return false;

    String path(requestUri);
    path.replace(_uri, _path);

    String contentType = getContentType(path);

    File f = SD.open(path);
    if (!f)
      return false;

    if (_cache_header.length() != 0)
      server.sendHeader("Cache-Control", _cache_header);

    server.streamFile(f, contentType);
    return true;
  }

  static String getContentType(const String &path)
  {
    char buff[sizeof(mimeTable[0].mimeType)];
    // Check all entries but last one for match, return if found
    for (size_t i = 0; i < sizeof(mimeTable) / sizeof(mimeTable[0]) - 1; i++)
    {
      if (path.endsWith(String(mimeTable[i].endsWith)))
      {
        return String(mimeTable[i].mimeType);
      }
    }
    // Fall-through and just return default type
    return String(mimeTable[3].mimeType);
  }

  static bool exists(const String &path)
  {
    return SD.exists(path.c_str());
  }

protected:
  String _uri;
  String _path;
  String _cache_header;
  bool _isFile;
  size_t _baseUriLength;
};

#endif
