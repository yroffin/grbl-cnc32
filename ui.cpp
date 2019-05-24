#include "ui.hpp"
#include "grbl-ctrl.hpp"
#include "storage-ctrl.hpp"

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
    this->background = TFT_BLACK;
    log_i("TFT_Screen allocated ...");
}

// init screen
void TFT_Screen::init()
{
    // Setup TFT
    pinMode(TFT_LED_PIN, OUTPUT);
    digitalWrite(TFT_LED_PIN, HIGH);

    log_i("TFT_Screen init ...");
    this->tft->init(); // Initialise l'écran avec les pins définies dans setup
    // Set the rotation before we calibrate
    this->tft->setRotation(1); // normally, this is already done in this->tft->int() but it is not clear how is rotation set (probably 0); so it can be usefull to change it here

    log_i("TFT_Screen calibration ...");
    calibrate(); // call screen calibration

    this->TFT_Widget::init(0, 0, 0, 320, 240);

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

    // First render
    log_i("TFT_Screen render ...");
    draw();
}

boolean TFT_Screen::getTouch(int16_t *x, int16_t *y)
{
    return this->tft->getTouch(x, y, 800);
}

// menu layer
TFT_LayerMenu::TFT_LayerMenu(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y, _w, _h)
{
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, "Menu", 0, 0);
    this->add(this->title);
    this->a = new TFT_Button(WIDGET_ID_LAYER_MENU_BTNA, "move", 8, 14);
    this->add(this->a);
    this->b = new TFT_Button(WIDGET_ID_LAYER_MENU_BTNB, "stat", 8, 14 + 44);
    this->add(this->b);
    this->c = new TFT_Button(WIDGET_ID_LAYER_MENU_BTNC, "files", 8, 14 + 44 * 2);
    this->add(this->c);
    this->d = new TFT_Button(WIDGET_ID_LAYER_MENU_BTND, "admin", 8, 14 + 44 * 3);
    this->add(this->d);
}

// Notification
void TFT_LayerMenu::notify(const Event *event)
{
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_MENU_BTNA)
    {
        TFT_Screen::instance()->control->show();
        TFT_Screen::instance()->statistic->hide();
        TFT_Screen::instance()->file->hide();
        TFT_Screen::instance()->admin->hide();
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_MENU_BTNB)
    {
        TFT_Screen::instance()->control->hide();
        TFT_Screen::instance()->statistic->show();
        TFT_Screen::instance()->file->hide();
        TFT_Screen::instance()->admin->hide();
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_MENU_BTNC)
    {
        TFT_Screen::instance()->control->hide();
        TFT_Screen::instance()->statistic->hide();
        TFT_Screen::instance()->file->show();
        TFT_Screen::instance()->admin->hide();
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_MENU_BTND)
    {
        TFT_Screen::instance()->control->hide();
        TFT_Screen::instance()->statistic->hide();
        TFT_Screen::instance()->file->hide();
        TFT_Screen::instance()->admin->show();
    }
    // Dispatch to layers
    this->TFT_Widget::notify(event);
}

// control layer
TFT_LayerAdmin::TFT_LayerAdmin(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y, _w, _h)
{
    this->group = new TFT_Group(WIDGET_ID_DEFAULT, "Admin", 0, 0, 265, 240);
    this->add(this->group);
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, "Control", 0, 0);
    this->group->add(this->title);
    this->print = new TFT_Button(WIDGET_ID_LAYER_BTN_PRINT, "Print", 0, 14);
    this->group->add(print);
    this->unlock = new TFT_Button(WIDGET_ID_LAYER_ADM_UNLOCK, "Unlock", 44, 14);
    this->group->add(this->unlock);
    this->reset = new TFT_Button(WIDGET_ID_LAYER_ADM_RESET, "Reset", 44 * 2, 14);
    this->group->add(this->reset);
    this->status = new TFT_Button(WIDGET_ID_LAYER_ADM_STATUS, "Status", 44 * 3, 14);
    this->group->add(this->status);
    this->pause = new TFT_Button(WIDGET_ID_LAYER_ADM_PAUSE, "Pause", 44 * 4, 14);
    this->group->add(this->pause);
    this->resume = new TFT_Button(WIDGET_ID_LAYER_ADM_RESUME, "Resume", 44 * 5, 14);
    this->group->add(this->resume);
    this->grblCommand = new TFT_Console(WIDGET_ID_DEFAULT, "grbl", 12, 0, 60, 265, 180);
    this->group->add(this->grblCommand);
}

void TFT_LayerAdmin::grblInputConsole(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char message[128];
    vsprintf(message, format, args);
    va_end(args);
    this->grblCommand->write(message);
}

void TFT_LayerAdmin::grblOutputConsole(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char message[128];
    vsprintf(message, format, args);
    va_end(args);
    this->grblCommand->write(message);
}

// control layer
TFT_LayerControl::TFT_LayerControl(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y, _w, _h)
{
    this->group = new TFT_Group(WIDGET_ID_DEFAULT, "Control", 0, 0, 265, 240);
    this->add(this->group);
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, "Control", 0, 0);
    this->group->add(this->title);
    this->setx = new TFT_Button(WIDGET_ID_LAYER_CTRL_SETX, "SetX", 0, 14);
    this->group->add(this->setx);
    this->sety = new TFT_Button(WIDGET_ID_LAYER_CTRL_SETY, "SetY", 44, 14);
    this->group->add(this->sety);
    this->setz = new TFT_Button(WIDGET_ID_LAYER_CTRL_SETZ, "SetZ", 44 * 2, 14);
    this->group->add(this->setz);
    this->setall = new TFT_Button(WIDGET_ID_LAYER_CTRL_SETALL, "SetAll", 44 * 3, 14);
    this->group->add(this->setall);
    this->home = new TFT_Button(WIDGET_ID_LAYER_CTRL_HOME, "Home", 44 * 4, 14);
    this->group->add(this->home);
    this->joystick = new TFT_Joystick(WIDGET_ID_LAYER_CTRL_JOYSTICK, "Control", 0, 58, 265, 140);
    this->group->add(this->joystick);
}

// stat layer
TFT_LayerStatistic::TFT_LayerStatistic(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y, _w, _h)
{
    this->group = new TFT_Group(WIDGET_ID_DEFAULT, "Statistics", 0, 0, 265, 240);
    this->add(this->group);
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, "Stat", 0, 0);
    this->group->add(this->title);
    this->grblStatusLabel = new TFT_Label(WIDGET_ID_DEFAULT, "status", 0, 14);
    this->group->add(this->grblStatusLabel);
    this->grblStatusValue = new TFT_Label(WIDGET_ID_LAYER_STAT_GRBL_STATUS, "...", 40, 14);
    this->group->add(this->grblStatusValue);

    this->grblIoStatus = new TFT_Label(WIDGET_ID_DEFAULT, "I/O", 0, 14 * 2);
    this->group->add(this->grblIoStatus);
    this->grblIoStatusValues = new TFT_Label(WIDGET_ID_LAYER_STAT_GRBL_IO, "...", 40, 14 * 2);
    this->group->add(this->grblIoStatusValues);

    this->grblMpos = new TFT_Label(WIDGET_ID_DEFAULT, "MPos", 0, 14 * 3);
    this->group->add(this->grblMpos);
    this->grblMposValue = new TFT_Label(WIDGET_ID_LAYER_STAT_GRBL_IO, "...", 40, 14 * 3);
    this->group->add(this->grblMposValue);

    this->grblWpos = new TFT_Label(WIDGET_ID_DEFAULT, "WPos", 0, 14 * 4);
    this->group->add(this->grblWpos);
    this->grblWposValue = new TFT_Label(WIDGET_ID_DEFAULT, "...", 40, 14 * 4);
    this->group->add(this->grblWposValue);

    this->console = new TFT_Console(WIDGET_ID_DEFAULT, "output", 6, 0, 150, 265, 90);
    this->group->add(this->console);
}

// Update screen status
void TFT_LayerStatistic::outputConsole(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[128];
    vsprintf(buffer, format, args);
    va_end(args);
    this->console->write(buffer);
}

// Notification
void TFT_LayerStatistic::notify(const Event *event)
{
    // Handle event on screen level
    if (event->type == EVENT_GRBL_STATUS)
    {
        this->grblStatusValue->setLabel("[%s]", event->message);
    }
    if (event->type == EVENT_MPOS)
    {
        this->grblMposValue->setLabel("x:%f y:%f z:%f", event->fvalue.f1, event->fvalue.f2, event->fvalue.f3);
    }
    if (event->type == EVENT_WPOS)
    {
        this->grblWposValue->setLabel("x:%f y:%f z:%f", event->fvalue.f1, event->fvalue.f2, event->fvalue.f3);
    }
    // Dispatch to layers
    this->TFT_Widget::notify(event);
}

// files layer
TFT_LayerFile::TFT_LayerFile(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y, _w, _h)
{
    this->group = new TFT_Group(WIDGET_ID_DEFAULT, "Statistics", 0, 0, 265, 240);
    this->add(this->group);
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, "Files", 0, 0);
    this->group->add(title);
    this->dir = new TFT_Label(WIDGET_ID_DEFAULT, "Dir", 0, 14);
    this->group->add(dir);
    this->cwd = new TFT_Label(WIDGET_ID_DEFAULT, "", 40, 14);
    this->group->add(cwd);
    this->file = new TFT_Label(WIDGET_ID_DEFAULT, "File", 0, 28);
    this->group->add(file);
    this->cwf = new TFT_Label(WIDGET_ID_DEFAULT, "", 40, 28);
    this->group->add(cwf);
    this->misc = new TFT_Label(WIDGET_ID_DEFAULT, "Misc", 0, 42);
    this->group->add(misc);
    this->miscValue = new TFT_Label(WIDGET_ID_DEFAULT, "", 40, 42);
    this->group->add(miscValue);
    this->files = new TFT_FileGrid(WIDGET_ID_LAYER_FILE_LIST, "files", 0, 54, 265, 160);
    this->group->add(files);
}

// Notification
void TFT_LayerFile::notify(const Event *event)
{
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_BTN_PRINT)
    {
        GrblCtrl::instance()->print(this->cwf->getLabel());
    }
    if (event->type == showComponent && event->target == WIDGET_ID_LAYER_FILE)
    {
        this->refresh();
    }
    if (event->type == fileGrid)
    {
        this->refresh();
    }
    if (event->type == fileSelect)
    {
        this->cwf->setLabel(event->message);
        this->invalidated = true;
    }
    this->TFT_Widget::notify(event);
}

// Notification
void TFT_LayerFile::refresh()
{
    this->files->clear();
    this->miscValue->setLabel("offset: %d", this->files->offset);
    this->files->count = StorageCtrl::instance()->getCount();
    int16_t files = min(this->files->count, 3);
    this->files->set(0, "..");
    for (int i = 0; i < files; i++)
    {
        this->files->set(1 + i, StorageCtrl::instance()->getEntries(i + this->files->offset)->getPath());
    }
}

// touch calibration
void TFT_Screen::calibrate()
{
    uint16_t calData[5] = {386, 3530, 220, 3627, 7};
    uint8_t calDataOK = 1;

    if (calDataOK)
    {
        // calibration data valid
        this->tft->setTouch(calData);
    }
    else
    {
        // data not valid so recalibrate
        this->tft->fillScreen(SCREEN_BACKGROUND);
        this->tft->setCursor(20, 0);
        this->tft->setTextFont(2);
        this->tft->setTextSize(1);
        this->tft->setTextColor(SCREEN_HEADER_TEXT, SCREEN_BACKGROUND);

        this->tft->println(__TOUCH_CORNER);

        this->tft->setTextFont(1);
        this->tft->println();

        this->tft->calibrateTouch(calData, SCREEN_ALERT_TEXT, SCREEN_BACKGROUND, 15);
        this->tft->setTextColor(SCREEN_NORMAL_TEXT, SCREEN_BACKGROUND);
        this->tft->println(__CAL_COMPLETED);
        //this->tft->printf("Cal data %d %d %d %d %d\n", calData[0], calData[1], calData[2], calData[3], calData[4]);

        TFT_Widget::init(WIDGET_ID_LAYER_MENU, 0, 0, calData[0], calData[1]);
    }
}

// Update screen status
void TFT_Screen::outputConsole(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[128];
    vsprintf(buffer, format, args);
    va_end(args);
    this->statistic->outputConsole("> %s", buffer);
}

// Update screen printing
void TFT_Screen::grblInputConsole(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[128];
    vsprintf(buffer, format, args);
    va_end(args);
    this->admin->grblInputConsole(buffer);
}

// Update screen printing
void TFT_Screen::grblOutputConsole(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[128];
    vsprintf(buffer, format, args);
    va_end(args);
    this->admin->grblOutputConsole(buffer);
}

// Notify event
void TFT_Screen::notify(const Event *event)
{
    // Dispatch to layers
    this->TFT_Widget::notify(event);
}
