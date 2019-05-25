#include "i18n-ctrl.hpp"

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
