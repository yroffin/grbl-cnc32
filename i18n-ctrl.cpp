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

I18nCtrl::I18nCtrl() : JsonStore(i18n)
{
}

void I18nCtrl::setup()
{
    this->load("/i18n_enUS.json");
    log_i("%d std keys loaded ...", this->i18n["std"].size());
    log_i("%d grbl keys loaded ...", this->i18n["grbl"].size());
}

const char *I18nCtrl::getKey(const char *k1, const char *k2)
{
    return this->getAsString(k1, k2, k2);
}

const char *I18nCtrl::grblError(int code)
{
    if (code < 0 || code >= this->i18n["grbl"].size())
    {
        code = 0;
    }
    return this->getAsString("grbl", code, "msg", "");
}

const char *I18nCtrl::std(int code)
{
    if (code < 0 || code >= this->i18n["std"].size())
    {
        code = 0;
    }
    return this->getAsString("std", code, "msg", "");
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
