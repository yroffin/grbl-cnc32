#include "i18n-ctrl.hpp"
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
