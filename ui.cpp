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

    this->TFT_Widget::init(0, 0, 0, 320, 200);

    // Init all layer
    log_i("TFT_Screen widget ...");
    this->menu = new TFT_LayerMenu(WIDGET_ID_LAYER_MENU, 0, 0);
    this->add(this->menu)->setVisible(true);
    this->control = new TFT_LayerControl(WIDGET_ID_LAYER_CTRL, 55, 0);
    this->add(this->control)->setVisible(false);
    this->statistic = new TFT_LayerStatistic(WIDGET_ID_LAYER_STAT, 55, 0);
    this->add(this->statistic)->setVisible(true);
    this->file = new TFT_LayerFile(WIDGET_ID_LAYER_FILE, 55, 0);
    this->add(this->file)->setVisible(false);

    // First render
    log_i("TFT_Screen render ...");
    render();
}

boolean TFT_Screen::getTouch(int16_t *x, int16_t *y)
{
    return this->tft->getTouch(x, y, 800);
}

// menu layer
TFT_LayerMenu::TFT_LayerMenu(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y)
{
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, "Menu", 0, 0);
    this->add(this->title);
    this->a = new TFT_Button(WIDGET_ID_LAYER_MENU_BTNA, "move", 8, 10);
    this->add(this->a);
    this->b = new TFT_Button(WIDGET_ID_LAYER_MENU_BTNB, "stat", 8, 60);
    this->add(this->b);
    this->c = new TFT_Button(WIDGET_ID_LAYER_MENU_BTNC, "files", 8, 110);
    this->add(this->c);
    this->footer = new TFT_Label(WIDGET_ID_LAYER_MENU_FOOTER, "......", 0, 225);
    this->add(this->footer);
}

// Notification
void TFT_LayerMenu::notify(const Event *event)
{
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_MENU_BTNA)
    {
        TFT_Screen::instance()->setInvalidated(true);
        this->setVisibleById(WIDGET_ID_LAYER_MENU, true);
        this->setVisibleById(WIDGET_ID_LAYER_CTRL, true);
        this->setVisibleById(WIDGET_ID_LAYER_STAT, false);
        this->setVisibleById(WIDGET_ID_LAYER_FILE, false);
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_MENU_BTNB)
    {
        TFT_Screen::instance()->setInvalidated(true);
        this->setVisibleById(WIDGET_ID_LAYER_MENU, true);
        this->setVisibleById(WIDGET_ID_LAYER_CTRL, false);
        this->setVisibleById(WIDGET_ID_LAYER_STAT, true);
        this->setVisibleById(WIDGET_ID_LAYER_FILE, false);
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_MENU_BTNC)
    {
        TFT_Screen::instance()->setInvalidated(true);
        this->setVisibleById(WIDGET_ID_LAYER_MENU, true);
        this->setVisibleById(WIDGET_ID_LAYER_CTRL, false);
        this->setVisibleById(WIDGET_ID_LAYER_STAT, false);
        this->setVisibleById(WIDGET_ID_LAYER_FILE, true);
    }
    // Dispatch to layers
    this->TFT_Widget::notify(event);
}

// control layer
TFT_LayerControl::TFT_LayerControl(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y)
{
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, "Control", 0, 0);
    add(this->title);
    this->home = new TFT_Button(WIDGET_ID_LAYER_CTRL_HOME, "Home", 0, 14);
    add(this->home);
    this->unlock = new TFT_Button(WIDGET_ID_LAYER_CTRL_UNLOCK, "Unlock", 50, 14);
    add(this->unlock);
    this->reset = new TFT_Button(WIDGET_ID_LAYER_CTRL_RESET, "Reset", 100, 14);
    add(this->reset);
    this->status = new TFT_Button(WIDGET_ID_LAYER_CTRL_STATUS, "Status", 150, 14);
    add(this->status);
    this->pause = new TFT_Button(WIDGET_ID_LAYER_CTRL_PAUSE, "Pause", 200, 14);
    add(this->pause);
    this->resume = new TFT_Button(WIDGET_ID_LAYER_CTRL_RESUME, "Resume", 200, 64);
    add(this->resume);
    this->joystick = new TFT_Joystick(WIDGET_ID_LAYER_CTRL_JOYSTICK, "Control", 0, 64);
    add(this->joystick);
}

// stat layer
TFT_LayerStatistic::TFT_LayerStatistic(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y)
{
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, "Stat", 0, 0);
    this->add(this->title);
    this->grblStatusLabel = new TFT_Label(WIDGET_ID_DEFAULT, "status", 0, 16);
    this->add(this->grblStatusLabel);
    this->grblStatusValue = new TFT_Label(WIDGET_ID_LAYER_STAT_GRBL_STATUS, "...", 40, 16);
    this->add(this->grblStatusValue);
    this->grblIoStatus = new TFT_Label(WIDGET_ID_DEFAULT, "I/O", 0, 29);
    this->add(this->grblIoStatus);
    this->grblIoStatusValues = new TFT_Label(WIDGET_ID_LAYER_STAT_GRBL_IO, "...", 40, 29);
    this->add(this->grblIoStatusValues);
    this->console = new TFT_Console(WIDGET_ID_DEFAULT, "console", 0, 70);
    this->add(this->console);
}

// Notification
void TFT_LayerStatistic::notify(const Event *event)
{
    // Handle event on screen level
    if (event->type == grblStatus)
    {
        this->grblStatusValue->setLabel(event->message);
    }
    // Dispatch to layers
    this->TFT_Widget::notify(event);
}

void TFT_LayerStatistic::write(const char *message)
{
    this->console->write(message);
}

void onLeft(TFT_FileGrid *p)
{
    log_i("left");
}

void onRight(TFT_FileGrid *p)
{
    log_i("right");
}

// files layer
TFT_LayerFile::TFT_LayerFile(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : TFT_Layer(_id, _x, _y)
{
    this->title = new TFT_Label(WIDGET_ID_DEFAULT, "Files", 0, 0);
    this->add(title);
    this->dir = new TFT_Label(WIDGET_ID_DEFAULT, "Dir", 0, 14);
    this->add(dir);
    this->cwd = new TFT_Label(WIDGET_ID_DEFAULT, "", 40, 14);
    this->add(cwd);
    this->file = new TFT_Label(WIDGET_ID_DEFAULT, "File", 0, 28);
    this->add(file);
    this->cwf = new TFT_Label(WIDGET_ID_DEFAULT, "", 40, 28);
    this->add(cwf);
    this->files = new TFT_FileGrid(WIDGET_ID_LAYER_FILE_LIST, onLeft, onRight, "files", 0, 70);
    this->add(files);
}

// Notification
void TFT_LayerFile::notify(const Event *event)
{
    if (event->type == show && event->target == WIDGET_ID_LAYER_FILE)
    {
        this->files->clear();
        this->files->set(0, "..");
        this->files->set(1, "b");
        this->files->set(2, "c");
        this->files->set(3, "d");
    }
    this->TFT_Widget::notify(event);
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
void TFT_Screen::status(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[128];
    vsprintf(buffer, format, args);
    va_end(args);
    this->setLabelById(WIDGET_ID_LAYER_MENU_FOOTER, buffer);
    this->statistic->write(buffer);
}

// Notify event
void TFT_Screen::notify(const Event *event)
{
    // Dispatch to layers
    this->TFT_Widget::notify(event);
}

// Render
void TFT_Screen::render()
{
    // clear if invalidated
    if (this->invalidated)
    {
        this->tft->fillScreen(TFT_BLACK);
        this->invalidated = false;
    }
    // display
    this->TFT_Widget::render();
}
