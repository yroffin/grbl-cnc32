#include "i18n-ctrl.hpp"
#include "config.h"
#include "utils.hpp"

// I18nCtrl controller
I18nCtrl *__instance_i18n = 0;

// singleton
I18nCtrl *I18nCtrl::instance()
{
    if (__instance_i18n == 0)
    {
        __instance_i18n = new I18nCtrl();
    }
    return __instance_i18n;
}

I18nCtrl::I18nCtrl()
{
}

void I18nCtrl::setup()
{
    this->loadi18n("/i18n_enUS.json");
}

// Loads the i18n file
void I18nCtrl::loadi18n(const char *filename)
{
    // Open file for reading
    File file = SD.open(filename);

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(this->i18n, file);
    if (error)
        log_i("Failed to read file %s, using default configuration", filename);

    log_i("%d std keys loaded ...", this->i18n["std"].size());
    log_i("%d grbl keys loaded ...", this->i18n["grbl"].size());

    // Close the file (Curiously, File's destructor doesn't close the file)
    file.close();
}

const char *I18nCtrl::grblError(int code)
{
    if (code < 0 || code >= this->i18n["grbl"].size())
    {
        code = 0;
    }
    return this->i18n["grbl"].getElement(code).getMember("msg").as<String>().c_str();
}

const char *I18nCtrl::std(int code)
{
    if (code < 0 || code >= this->i18n["std"].size())
    {
        code = 0;
    }
    return this->i18n["std"].getElement(code).getMember("msg").as<String>().c_str();
}

// translate message
const char *I18nCtrl::translate(int type, int code, ...)
{
    va_list args;
    va_start(args, code);
    const char *format;
    switch (type)
    {
    case I18N_STD:
        format = std(code);
        break;
    case I18N_GRBL:
        format = grblError(code);
        break;
    }
    // use utils buffer to protect memory
    vsprintf(Utils::vsprintfBuffer(), format, args);
    Utils::strcpy(this->i18n_message, Utils::vsprintfBuffer(), MAXSIZE_OF_I18N_MESSAGE);
    va_end(args);
    return this->i18n_message;
}
