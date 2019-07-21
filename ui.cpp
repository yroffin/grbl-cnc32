#include "ui.hpp"
#include "grbl-ctrl.hpp"
#include "json-config.hpp"
#include "storage-ctrl.hpp"
#include "i18n-ctrl.hpp"

// screen controller
TFT_Screen *__instance_tft = 0;
extern TFT_eSPI _tft;

// singleton
TFT_Screen *TFT_Screen::instance()
{
    if (__instance_tft == 0)
    {
        __instance_tft = new TFT_Screen();
    }
    return __instance_tft;
}

TFT_Screen::TFT_Screen()
{
    // create instance for touchscreeen
    this->tft = &_tft;
    this->background = this->getKeyAsInt("tft", "background", TFT_BLACK);
    log_i("TFT_Screen allocated ...");
}

// init screen
void TFT_Screen::prepare()
{
    // Setup TFT
    pinMode(TFT_LED_PIN, OUTPUT);
    digitalWrite(TFT_LED_PIN, HIGH);

    log_i("TFT_Screen init ...");
    this->tft->init(); // Initialise l'écran avec les pins définies dans setup
    // Set the rotation before we calibrate
    this->tft->setRotation(1); // normally, this is already done in this->tft->int() but it is not clear how is rotation set (probably 0); so it can be usefull to change it here
    this->tft->fillRect(0, 0, 400, 400, this->getKeyAsInt("tft", "background", TFT_BLACK));
}

// init screen
void TFT_Screen::init()
{
    log_i("TFT_Screen calibration ...");
    calibrate(); // call screen calibration

    this->TFT_Widget::init(0, "", 0, 0, 320, 240);

    // Init all layer
    log_i("TFT_Screen widget ...");
    this->menu = new TFT_LayerMenu(WIDGET_ID_LAYER_MENU, 0, 0, 50, 50);
    this->add(this->menu)->show();
    // Layer
    this->admin = new TFT_LayerAdmin(WIDGET_ID_LAYER_ADM, 55, 0, 50, 100);
    this->add(this->admin)->hide();
    this->control = new TFT_LayerControl(WIDGET_ID_LAYER_CTRL, 55, 0, 50, 100);
    this->add(this->control)->hide();
    this->statistic = new TFT_LayerStatistic(WIDGET_ID_LAYER_STAT, 55, 0, 50, 100);
    this->add(this->statistic)->show();
    this->file = new TFT_LayerFile(WIDGET_ID_LAYER_FILE, 55, 0, 0, 0);
    this->add(this->file)->hide();
    this->cmd = new TFT_LayerCmd(WIDGET_ID_DEFAULT, 55, 0, 0, 0);
    this->add(this->cmd)->hide();
    this->dialog = new TFT_LayerDialog(WIDGET_ID_DEFAULT, 55, 0, 0, 0);
    this->add(this->dialog)->hide();

    // First render
    log_i("TFT_Screen render ...");
    draw();
}

// init screen
void TFT_Screen::console(const char *text)
{
    this->tft->println(text);
}

boolean TFT_Screen::getTouch(int16_t *x, int16_t *y)
{
    return this->tft->getTouch(x, y, 800);
}

// touch calibration
void TFT_Screen::calibrate()
{
    JsonConfigCtrl *jsonConfig = JsonConfigCtrl::instance();
    uint16_t calData[5] = {442, 3449, 207, 3359, 7};
    //boolean calDataOK = jsonConfig->getAsArray("tft", "calibrate", calData, 5);
    boolean calDataOK = true;

    if (calDataOK)
    {
        // calibration data valid
        this->tft->setTouch(calData);
        log_i("Cal data %d %d %d %d %d", calData[0], calData[1], calData[2], calData[3], calData[4]);
    }
    else
    {
        this->tft->setCursor(20, 0);
        this->tft->setTextFont(1);

        this->tft->calibrateTouch(calData, this->getKeyAsInt("tft", "calColor", TFT_WHITE), this->getKeyAsInt("tft", "background", TFT_BLACK), 15);
        log_i("Cal data %d %d %d %d %d", calData[0], calData[1], calData[2], calData[3], calData[4]);

        TFT_Widget::init(WIDGET_ID_LAYER_MENU, "", 0, 0, calData[0], calData[1]);
        jsonConfig->setAsArray("tft", "calibrate", calData, 5);
        jsonConfig->write();
    }
}

// Update screen status
void TFT_Screen::outputConsole(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    // use utils buffer to protect memory
    vsprintf(Utils::vsprintfBuffer(), format, args);
    Utils::strcpy(this->log_message, Utils::vsprintfBuffer(), MAXSIZE_OF_LOG_MESSAGE);
    va_end(args);
    this->statistic->outputConsole(this->getKey("Misc", "CSL"), this->log_message);
}

// Update screen printing
void TFT_Screen::grblInputConsole(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    // use utils buffer to protect memory
    vsprintf(Utils::vsprintfBuffer(), format, args);
    Utils::strcpy(this->log_message, Utils::vsprintfBuffer(), MAXSIZE_OF_LOG_MESSAGE);
    va_end(args);
    this->admin->grblInputConsole(this->log_message);
}

// Update screen printing
void TFT_Screen::grblOutputConsole(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsprintf(this->log_message, format, args);
    va_end(args);
    this->admin->grblOutputConsole(this->log_message);
}

// Notify event
void TFT_Screen::notify(const Event *event)
{
    // Dispatch to layers
    this->TFT_Widget::notify(event);
}

void TFT_Screen::notifyWrite(uint16_t sz)
{
    if (this->menu && this->menu->status)
    {
        this->menu->status->notifyWrite(sz);
    }
}

void TFT_Screen::notifyBusy(boolean _busyState)
{
    if (this->menu && this->menu->status)
    {
        this->menu->status->notifyBusy(_busyState);
    }
}

void TFT_Screen::notifyWifiStatus(const char *status)
{
    if (this->menu && this->menu->status)
    {
        this->menu->status->notifyWifiStatus(status);
    }
}

void TFT_Screen::notifyPrintStatus(boolean isPrinting, int printed, int toPrint)
{
    if (this->menu && this->menu->status)
    {
        this->menu->status->notifyPrintStatus(isPrinting, printed, toPrint);
    }
}

// menu layer
TFT_LayerMenu::TFT_LayerMenu(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y, _w, _h)
{
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Menu", "MENU"), 0, 0);
    this->add(this->title);
    this->move = new TFT_Button(WIDGET_ID_LAYER_MENU_BTNA, this->getKey("Menu", "MV"), 8, 14, 40, 40);
    this->move->setEvent(EVENT_BTN_MOVE);
    this->add(this->move);
    this->stat = new TFT_Button(WIDGET_ID_LAYER_MENU_BTNB, this->getKey("Menu", "STA"), 8, 14 + 44, 40, 40);
    this->stat->setEvent(EVENT_BTN_STAT);
    this->add(this->stat);
    this->files = new TFT_Button(WIDGET_ID_LAYER_MENU_BTNC, this->getKey("Menu", "FIL"), 8, 14 + 44 * 2, 40, 40);
    this->files->setEvent(EVENT_BTN_FILES);
    this->add(this->files);
    this->admin = new TFT_Button(WIDGET_ID_LAYER_MENU_BTND, this->getKey("Menu", "ADM"), 8, 14 + 44 * 3, 40, 40);
    this->admin->setEvent(EVENT_BTN_ADM);
    this->add(this->admin);
    this->cmd = new TFT_Button(WIDGET_ID_LAYER_MENU_BTND, this->getKey("Menu", "CMD"), 8, 14 + 44 * 4, 40, 40);
    this->cmd->setEvent(EVENT_BTN_CMD);
    this->add(this->cmd);
    // Status bar
    this->status = new TFT_StatusBar(WIDGET_ID_DEFAULT, 0, 230);
    this->add(this->status);
}

// Notification
void TFT_LayerMenu::notify(const Event *event)
{
    if (event->type == EVENT_BTN_MOVE)
    {
        TFT_Screen::instance()->control->show();
        TFT_Screen::instance()->statistic->hide();
        TFT_Screen::instance()->file->hide();
        TFT_Screen::instance()->admin->hide();
        TFT_Screen::instance()->cmd->hide();
    }
    if (event->type == EVENT_BTN_STAT)
    {
        TFT_Screen::instance()->control->hide();
        TFT_Screen::instance()->statistic->show();
        TFT_Screen::instance()->file->hide();
        TFT_Screen::instance()->admin->hide();
        TFT_Screen::instance()->cmd->hide();
    }
    if (event->type == EVENT_BTN_FILES)
    {
        TFT_Screen::instance()->control->hide();
        TFT_Screen::instance()->statistic->hide();
        TFT_Screen::instance()->file->show();
        TFT_Screen::instance()->admin->hide();
        TFT_Screen::instance()->cmd->hide();
    }
    if (event->type == EVENT_BTN_ADM)
    {
        TFT_Screen::instance()->control->hide();
        TFT_Screen::instance()->statistic->hide();
        TFT_Screen::instance()->file->hide();
        TFT_Screen::instance()->admin->show();
        TFT_Screen::instance()->cmd->hide();
    }
    if (event->type == EVENT_BTN_CMD)
    {
        TFT_Screen::instance()->control->hide();
        TFT_Screen::instance()->statistic->hide();
        TFT_Screen::instance()->file->hide();
        TFT_Screen::instance()->admin->hide();
        TFT_Screen::instance()->cmd->show();
    }
    // Dispatch to layers
    this->TFT_Widget::notify(event);
}

// control layer
TFT_LayerControl::TFT_LayerControl(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y, _w, _h)
{
    this->group = new TFT_Group(WIDGET_ID_DEFAULT, this->getKey("Control", "CTRL"), 0, 0, 265, 230);
    this->add(this->group);
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Control", "CTRL"), 0, 0);
    this->group->add(this->title);
    this->setx = new TFT_Button(WIDGET_ID_LAYER_CTRL_SETX, this->getKey("Control", "SX"), 0, 14, 40, 40);
    this->setx->setEvent(EVENT_SETX);
    this->group->add(this->setx);
    this->sety = new TFT_Button(WIDGET_ID_LAYER_CTRL_SETY, this->getKey("Control", "SY"), 44, 14, 40, 40);
    this->sety->setEvent(EVENT_SETY);
    this->group->add(this->sety);
    this->setz = new TFT_Button(WIDGET_ID_LAYER_CTRL_SETZ, this->getKey("Control", "SZ"), 44 * 2, 14, 40, 40);
    this->setz->setEvent(EVENT_SETZ);
    this->group->add(this->setz);
    this->setall = new TFT_Button(WIDGET_ID_LAYER_CTRL_SETALL, this->getKey("Control", "SA"), 44 * 3, 14, 40, 40);
    this->setall->setEvent(EVENT_SETXYZ);
    this->group->add(this->setall);
    this->home = new TFT_Button(WIDGET_ID_LAYER_CTRL_HOME, this->getKey("Control", "HME"), 44 * 4, 14, 40, 40);
    this->group->add(this->home);
    this->joystick = new TFT_Joystick(WIDGET_ID_LAYER_CTRL_JOYSTICK, this->getKey("Control", "CTRL"), 0, 58, 265, 140);
    this->group->add(this->joystick);
}

// stat layer
TFT_LayerStatistic::TFT_LayerStatistic(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y, _w, _h)
{
    this->group = new TFT_Group(WIDGET_ID_DEFAULT, this->getKey("Stat", "STA"), 0, 0, 265, 14 * 5);
    this->add(this->group);
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Stat", "STA"), 0, 0);
    this->group->add(this->title);
    this->grblStatusLabel = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Stat", "ST"), 0, 14);
    this->group->add(this->grblStatusLabel);
    this->grblStatusValue = new TFT_Label(WIDGET_ID_LAYER_STAT_GRBL_STATUS, this->getKey("Stat", "NIL"), 40, 14);
    this->group->add(this->grblStatusValue);

    this->grblIoStatus = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Stat", "IO"), 0, 14 * 2);
    this->group->add(this->grblIoStatus);
    this->grblIoStatusValues = new TFT_Label(WIDGET_ID_LAYER_STAT_GRBL_IO, this->getKey("Stat", "NIL"), 40, 14 * 2);
    this->group->add(this->grblIoStatusValues);

    this->grblMpos = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Stat", "MP"), 0, 14 * 3);
    this->group->add(this->grblMpos);
    this->grblMposValue = new TFT_Label(WIDGET_ID_LAYER_STAT_GRBL_IO, this->getKey("Stat", "NIL"), 40, 14 * 3);
    this->group->add(this->grblMposValue);

    this->grblWpos = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Stat", "WP"), 0, 14 * 4);
    this->group->add(this->grblWpos);
    this->grblWposValue = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Stat", "NIL"), 40, 14 * 4);
    this->group->add(this->grblWposValue);

    this->groupNunchuk = new TFT_Group(WIDGET_ID_DEFAULT, this->getKey("Stat", "CHK"), 0, 14 * 5, 265, 14 * 5);
    this->add(this->groupNunchuk);

    this->nunchuk = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Stat", "CHK"), 0, 0);
    this->groupNunchuk->add(this->nunchuk);
    this->nunchukZ = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Stat", "ZERO"), 40, 0);
    this->groupNunchuk->add(this->nunchukZ);
    this->nunchukC = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Stat", "ZERO"), 70, 0);
    this->groupNunchuk->add(this->nunchukC);
    this->nunchukPlane = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Stat", "ZERO"), 100, 0);
    this->groupNunchuk->add(this->nunchukPlane);
    this->nunchukMove = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Stat", "NIL"), 40, 14);
    this->groupNunchuk->add(this->nunchukMove);
    this->nunchukLader = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Stat", "ZERO"), 100, 14);
    this->groupNunchuk->add(this->nunchukLader);

    this->console = new TFT_Console(WIDGET_ID_DEFAULT, this->getKey("Stat", "OUT"), 6, 0, 14 * 10, 265, 14 * 6);
    this->group->add(this->console);
}

// Update screen status
void TFT_LayerStatistic::outputConsole(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    // use utils buffer to protect memory
    vsprintf(Utils::vsprintfBuffer(), format, args);
    Utils::strcpy(this->log_message, Utils::vsprintfBuffer(), MAXSIZE_OF_LOG_MESSAGE);
    va_end(args);
    this->console->write(this->log_message);
}

// Notification
void TFT_LayerStatistic::notify(const Event *event)
{
    // Handle event on screen level
    if (event->type == EVENT_GRBL_STATUS)
    {
        switch (event->ivalue)
        {
        case GRBL_IDLE:
            this->grblStatusValue->setLabel("IDLE");
            break;
        case GRBL_RUN:
            this->grblStatusValue->setLabel("RUN");
            break;
        case GRBL_ALARM:
            this->grblStatusValue->setLabel("ALARM");
            break;
        case GRBL_CHECK:
            this->grblStatusValue->setLabel("CHECK");
            break;
        case GRBL_DOOR:
            this->grblStatusValue->setLabel("DOOR");
            break;
        case GRBL_HOLD:
            this->grblStatusValue->setLabel("HOLD");
            break;
        case GRBL_SLEEP:
            this->grblStatusValue->setLabel("SLEEP");
            break;
        default:
            this->grblStatusValue->setLabel("?????");
        }
    }
    if (event->type == EVENT_MPOS)
    {
        this->grblMposValue->setLabel(this->getKey("Stat", "GRBL"), event->fvalue.f1, event->fvalue.f2, event->fvalue.f3);
    }
    if (event->type == EVENT_WPOS)
    {
        this->grblWposValue->setLabel(this->getKey("Stat", "GRBL"), event->fvalue.f1, event->fvalue.f2, event->fvalue.f3);
    }
    if (event->type == NUNCHUK_Z)
    {
        this->nunchukZ->setLabel(this->getKey("Stat", "NKZ"), event->ivalue);
        this->nunchukLader->setLabel("");
        this->nunchukMove->setLabel("");
        this->groupNunchuk->setInvalidated(true);
    }
    if (event->type == NUNCHUK_C)
    {
        this->nunchukC->setLabel(this->getKey("Stat", "NKC"), event->ivalue);
        this->nunchukLader->setLabel("");
        this->nunchukMove->setLabel("");
        this->groupNunchuk->setInvalidated(true);
    }
    if (event->type == NUNCHUK_DATA)
    {
        this->nunchukPlane->setLabel(this->getKey("Stat", "NKXY"), event->touch.x, event->touch.y);
        this->nunchukLader->setLabel("");
        this->nunchukMove->setLabel("");
        this->groupNunchuk->setInvalidated(true);
    }
    // move button panel can also jog
    if (event->type == JOG_MOVEXY || event->type == JOG_MOVEZ)
    {
        switch (event->type)
        {
        case JOG_MOVEXY:
            this->nunchukLader->setLabel(this->getKey("Stat", "JGXY"), event->fvalue.f1, event->fvalue.f2);
            this->nunchukMove->setLabel(this->getKey("Stat", "MGXY"));
            break;
        case JOG_MOVEZ:
            this->nunchukLader->setLabel(this->getKey("Stat", "JGZ"), event->fvalue.f3);
            this->nunchukMove->setLabel(this->getKey("Stat", "MGZ"));
            break;
        }
        this->groupNunchuk->setInvalidated(true);
    }
    // Dispatch to layers
    this->TFT_Widget::notify(event);
}

// files layer
TFT_LayerFile::TFT_LayerFile(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y, _w, _h)
{
    this->group = new TFT_Group(WIDGET_ID_DEFAULT, this->getKey("Files", "FIL"), 0, 0, 265, 230);
    this->add(this->group);
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Files", "FIL"), 0, 0);
    this->group->add(title);
    this->dir = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Files", "DIR"), 0, 14);
    this->group->add(dir);
    this->cwd = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Files", "NIL"), 40, 14);
    this->group->add(cwd);
    this->file = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Files", "FN"), 0, 28);
    this->group->add(file);
    this->cwf = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Files", "NIL"), 40, 28);
    this->group->add(cwf);
    this->misc = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Files", "MC"), 0, 42);
    this->group->add(misc);
    this->miscValue = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Files", "NIL"), 40, 42);
    this->group->add(miscValue);
    this->sw = new TFT_Button(WIDGET_ID_DEFAULT, this->getKey("Files", "FIL"), 210, 6, 40, 40);
    this->sw->setEvent(SWITCH_SELECTED);
    this->group->add(this->sw);
    this->files = new TFT_FileGrid(WIDGET_ID_LAYER_FILE_LIST, this->getKey("Files", "FIL"), 0, 54, 265, 160);
    this->group->add(files);
}

// Notification
void TFT_LayerFile::notify(const Event *event)
{
    if (event->type == EVENT_START_PRINT)
    {
        GrblCtrl::instance()->print(this->cwf->getLabel());
    }
    if (event->type == EVENT_BTN_FILES)
    {
        this->refresh();
    }
    if (event->type == GRID_CHANGE)
    {
        this->refresh();
    }
    if (event->type == FILE_SELECTED)
    {
        this->cwd->setLabel("/");
        this->cwf->setLabel(event->message);
        this->invalidated = true;
        TFT_Screen::instance()->dialog->show(PRINT, this->cwf->getLabel(), I18nCtrl::instance()->translate(I18N_STD, this->getKey("Files", "PRT"), this->cwf->getLabel()));
    }
    if (event->type == SWITCH_SELECTED)
    {
        switch (StorageCtrl::instance()->toggle())
        {
        case COMMANDS:
            this->sw->setLabel(this->getKey("Files", "SWC"));
            break;
        case FILES:
            this->sw->setLabel(this->getKey("Files", "SWF"));
            break;
        }
        this->refresh();
    }
    this->TFT_Widget::notify(event);
}

// Notification
void TFT_LayerFile::refresh()
{
    this->files->clear();
    this->miscValue->setLabel(this->getKey("Files", "OFF"), this->files->offset);
    this->files->count = StorageCtrl::instance()->scan();
    for (int i = 0; i < this->files->maxLines; i++)
    {
        if ((i + this->files->offset) < this->files->count)
        {
            this->files->set(i, StorageCtrl::instance()->path(i + this->files->offset));
        }
    }
}

// admin layer
TFT_LayerAdmin::TFT_LayerAdmin(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y, _w, _h)
{
    this->group = new TFT_Group(WIDGET_ID_DEFAULT, this->getKey("Admin", "ADM"), 0, 0, 265, 230);
    this->add(this->group);
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Admin", "ADM"), 0, 0);
    this->group->add(this->title);
    this->print = new TFT_Button(WIDGET_ID_LAYER_BTN_PRINT, this->getKey("Admin", "PRT"), 0, 14, 40, 40);
    this->print->setEvent(EVENT_START_PRINT);
    this->group->add(print);
    this->unlock = new TFT_Button(WIDGET_ID_LAYER_ADM_UNLOCK, this->getKey("Admin", "ULK"), 44, 14, 40, 40);
    this->group->add(this->unlock);
    this->reset = new TFT_Button(WIDGET_ID_LAYER_ADM_RESET, this->getKey("Admin", "RST"), 44 * 2, 14, 40, 40);
    this->group->add(this->reset);
    this->status = new TFT_Button(WIDGET_ID_LAYER_ADM_STATUS, this->getKey("Admin", "STA"), 44 * 3, 14, 40, 40);
    this->group->add(this->status);
    this->pause = new TFT_Button(WIDGET_ID_LAYER_ADM_PAUSE, this->getKey("Admin", "PSE"), 44 * 4, 14, 40, 40);
    this->group->add(this->pause);
    this->resume = new TFT_Button(WIDGET_ID_LAYER_ADM_RESUME, this->getKey("Admin", "RES"), 44 * 5, 14, 40, 40);
    this->group->add(this->resume);
    this->grblCommand = new TFT_Console(WIDGET_ID_DEFAULT, this->getKey("Admin", "ADM"), 12, 0, 60, 265, 14 * 12);
    this->group->add(this->grblCommand);
}

void TFT_LayerAdmin::grblInputConsole(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    // use utils buffer to protect memory
    vsprintf(Utils::vsprintfBuffer(), format, args);
    Utils::strcpy(this->log_message, Utils::vsprintfBuffer(), MAXSIZE_OF_LOG_MESSAGE);
    va_end(args);
    this->grblCommand->write(this->log_message);
}

void TFT_LayerAdmin::grblOutputConsole(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    // use utils buffer to protect memory
    vsprintf(Utils::vsprintfBuffer(), format, args);
    Utils::strcpy(this->log_message, Utils::vsprintfBuffer(), MAXSIZE_OF_LOG_MESSAGE);
    va_end(args);
    this->grblCommand->write(this->log_message);
}

// command layer
TFT_LayerCmd::TFT_LayerCmd(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y, _w, _h)
{
    this->group = new TFT_Group(WIDGET_ID_DEFAULT, this->getKey("Cmd", "ADM"), 0, 0, 265, 230);
    this->add(this->group);
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Admin", "ADM"), 0, 0);
    this->group->add(this->title);

    // scan all files
    StorageCtrl::instance()->commands();
    int16_t count = StorageCtrl::instance()->scan();
    for (int i = 0; i < sizeof(this->commands); i++)
    {
        //this->commands[i] = new TFT_Button(WIDGET_ID_DEFAULT, StorageCtrl::instance()->path(i), 0 * 44 * (i % 6), 14, 40, 40);
        //this->group->add(this->commands[i]);
    }
}

// dialog layer
TFT_LayerDialog::TFT_LayerDialog(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y, _w, _h)
{
    this->group = new TFT_Group(WIDGET_ID_DEFAULT, this->getKey("Dialog", "DLG"), 0, 0, 265, 230);
    this->add(this->group);
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, this->getKey("Dialog", "DLG"), 30, 60);
    this->group->add(this->title);
    this->ok = new TFT_Button(WIDGET_ID_DIALOG_OK, this->getKey("Dialog", "OK"), 90, 90, 40, 40);
    this->ok->setEvent(EVENT_DLG_OK);
    this->group->add(ok);
    this->cancel = new TFT_Button(WIDGET_ID_DIALOG_CANCEL, this->getKey("Dialog", "CANCEL"), 134, 90, 40, 40);
    this->cancel->setEvent(EVENT_DLG_CANCEL);
    this->group->add(cancel);
}

// Notification
void TFT_LayerDialog::notify(const Event *event)
{
    if (event->type == EVENT_DLG_OK)
    {
        this->hide();
        GrblCtrl::instance()->print(this->data);
        switch (flow)
        {
        case PRINT:
            TFT_Screen::instance()->control->hide();
            TFT_Screen::instance()->statistic->hide();
            TFT_Screen::instance()->file->hide();
            TFT_Screen::instance()->admin->show();
            break;
        }
    }
    if (event->type == EVENT_DLG_CANCEL)
    {
        this->hide();
        switch (flow)
        {
        case PRINT:
            TFT_Screen::instance()->control->hide();
            TFT_Screen::instance()->statistic->hide();
            TFT_Screen::instance()->file->show();
            TFT_Screen::instance()->admin->hide();
            break;
        }
    }
    this->TFT_Widget::notify(event);
}

// Show
void TFT_LayerDialog::show(DialogFlow _flow, const char *_data, const char *_title)
{
    this->TFT_Widget::show();
    this->flow = flow;
    this->title->setLabel(_title);
    Utils::strcpy(this->data, _data, sizeof(this->data));
    TFT_Screen::instance()->control->hide();
    TFT_Screen::instance()->statistic->hide();
    TFT_Screen::instance()->file->hide();
    TFT_Screen::instance()->admin->hide();
}
