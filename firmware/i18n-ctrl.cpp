#include "json-config.hpp"
#include "i18n-ctrl.hpp"
#include "vars.hpp"
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
    this->load(JsonConfigCtrl::instance()->lang);
    log_i("%d std keys loaded ...", this->i18n["Std"].size());
    log_i("%d grbl keys loaded ...", this->i18n["Grbl"].size());
}

// find key
const char *I18nCtrl::getKey(const char *k1, const char *k2)
{
    return this->getAsString(k1, k2, k2);
}

// translate message
const char *I18nCtrl::translate(const char *section, const char *code, ...)
{
    va_list args;
    va_start(args, code);
    // use utils buffer to protect memory
    vsprintf(Utils::vsprintfBuffer(), this->getKey(section, code), args);
    Utils::strcpy(this->i18n_message, Utils::vsprintfBuffer(), MAXSIZE_OF_I18N_MESSAGE);
    va_end(args);
    return this->i18n_message;
}
