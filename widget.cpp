#include "widget.hpp"
#include "evt-ctrl.hpp"

// screen controller
TFT_Screen *__instance_tft = 0;

// singleton
TFT_Screen *TFT_Screen::instance()
{
    if (__instance_tft == 0)
    {
        __instance_tft = new TFT_Screen();
    }
    return __instance_tft;
}

TFT_eSPI _tft = TFT_eSPI();

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
    menuLayer();
    controlLayer();
    statLayer();

    // First render
    log_i("TFT_Screen render ...");
    render();
}

boolean TFT_Screen::getTouch(int16_t *x, int16_t *y)
{
    return this->tft->getTouch(x, y, 800);
}

// menu layer
void TFT_Screen::menuLayer()
{
    log_i("TFT_Screen menu layer ...");
    // Init all layer
    static TFT_Layer layer(WIDGET_ID_LAYER_MENU, 0, 0);
    add(&layer)->setVisible(true);
    static TFT_Label title(WIDGET_ID_DEFAULT, "Menu", 0, 0);
    layer.add(&title);
    static TFT_Button a(WIDGET_ID_LAYER_MENU_BTNA, "move", 8, 10);
    layer.add(&a);
    static TFT_Button b(WIDGET_ID_LAYER_MENU_BTNB, "stat", 8, 60);
    layer.add(&b);
    static TFT_Label footer(WIDGET_ID_LAYER_MENU_FOOTER, "......", 0, 225);
    layer.add(&footer);
}

// control layer
void TFT_Screen::controlLayer()
{
    log_i("TFT_Screen control layer ...");
    // Init all layer
    static TFT_Layer layer(WIDGET_ID_LAYER_CTRL, 55, 0);
    add(&layer)->setVisible(false);
    static TFT_Label title(WIDGET_ID_DEFAULT, "Control", 0, 0);
    layer.add(&title);
    static TFT_Button home(WIDGET_ID_LAYER_CTRL_HOME, "Home", 0, 14);
    layer.add(&home);
    static TFT_Button unlock(WIDGET_ID_LAYER_CTRL_UNLOCK, "Unlock", 50, 14);
    layer.add(&unlock);
    static TFT_Button reset(WIDGET_ID_LAYER_CTRL_RESET, "Reset", 100, 14);
    layer.add(&reset);
    static TFT_Button status(WIDGET_ID_LAYER_CTRL_STATUS, "Status", 150, 14);
    layer.add(&status);
    static TFT_Button pause(WIDGET_ID_LAYER_CTRL_PAUSE, "Pause", 200, 14);
    layer.add(&pause);
    static TFT_Button resume(WIDGET_ID_LAYER_CTRL_RESUME, "Resume", 200, 64);
    layer.add(&resume);
    static TFT_Joystick joystick(WIDGET_ID_LAYER_CTRL_JOYSTICK, "Control", 0, 40);
    layer.add(&joystick);
}

// stat layer
void TFT_Screen::statLayer()
{
    log_i("TFT_Screen statistics layer ...");
    // Init all layer
    static TFT_Layer layer(WIDGET_ID_LAYER_STAT, 55, 0);
    add(&layer)->setVisible(true);
    static TFT_Label title(WIDGET_ID_DEFAULT, "Stat", 0, 0);
    layer.add(&title);
    static TFT_Label grblStatus(WIDGET_ID_DEFAULT, "status", 0, 16);
    layer.add(&grblStatus);
    static TFT_Label grblStatusValue(WIDGET_ID_LAYER_STAT_GRBL_STATUS, "...", 40, 16);
    layer.add(&grblStatusValue);
    static TFT_Label grblIoStatus(WIDGET_ID_DEFAULT, "I/O", 0, 29);
    layer.add(&grblIoStatus);
    static TFT_Label grblIoStatusValues(WIDGET_ID_LAYER_STAT_GRBL_IO, "...", 40, 29);
    layer.add(&grblIoStatusValues);
    static TFT_Console _console(WIDGET_ID_DEFAULT, "console", 0, 70);
    this->console = &_console;
    layer.add(&_console);
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

// Submit event localy created by widget
void TFT_Screen::submit(Event *event)
{
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_MENU_BTNA)
    {
        this->invalidated = true;
        this->setVisibleById(WIDGET_ID_LAYER_MENU, true);
        this->setVisibleById(WIDGET_ID_LAYER_CTRL, true);
        this->setVisibleById(WIDGET_ID_LAYER_STAT, false);
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_MENU_BTNB)
    {
        this->invalidated = true;
        this->setVisibleById(WIDGET_ID_LAYER_MENU, true);
        this->setVisibleById(WIDGET_ID_LAYER_CTRL, false);
        this->setVisibleById(WIDGET_ID_LAYER_STAT, true);
    }
    // Submit event to other controller
    GrblCtrl::instance()->submit(event);
    // Display it
    log_i("event: %d sender: %08x", event->type, event->sender);
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
    this->console->write(buffer);
}

// Notify event
void TFT_Screen::notify(const Event *event)
{
    // Handle event on screen level
    if (event->type == grblStatus)
    {
        this->setLabelById(WIDGET_ID_LAYER_STAT_GRBL_STATUS, event->message);
    }

    // Dispatch to layers
    this->TFT_Widget::notify(event);
}

// Notify each event to each layer
void TFT_Screen::dispatch()
{
    int16_t maxEvents = EvtCtrl::instance()->countEvents();
    for (int e = 0; e < maxEvents; e++)
    {
        notify(EvtCtrl::instance()->getEvent(e));
    }
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

// Global for internal use
TFT_Widget *__registry[128];
int8_t __registryCount = 0;

// constructor
TFT_Widget::TFT_Widget()
{
    // Global registry for all widget
    __registry[__registryCount++] = this;
}

// Init this widget
void TFT_Widget::init(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    this->x = _x;
    this->y = _y;
    this->w = _w;
    this->h = _h;
    sprintf(this->name, "widget-%08x", _id);
    this->id = _id;
    this->visible = true;
    this->tft = &_tft;
}

// Find widget by its id
TFT_Widget *TFT_Widget::findById(int16_t _id)
{
    for (int8_t i = 0; i < __registryCount; i++)
    {
        if (__registry[i]->id == _id)
        {
            return __registry[i];
        }
    }
}

void TFT_Widget::setLabelById(int16_t _id, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char buffer[128];
    vsprintf(buffer, format, args);
    va_end(args);
    findById(_id)->setLabel(buffer);
}

void TFT_Widget::setLabel(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsprintf(this->label, format, args);
    va_end(args);
    // draw needed
    this->invalidated = true;
}

void TFT_Widget::setVisibleById(int16_t _id, bool _active)
{
    findById(_id)->setVisible(_active);
}

void TFT_Widget::setVisible(bool _active)
{
    this->visible = _active;
    if (_active)
    {
        // invalidate all children too
        this->setInvalidated(true);
    }
}

void TFT_Widget::setInvalidated(bool invalidate)
{
    this->invalidated = invalidate;
    for (int index = 0; index < child; index++)
    {
        children[index]->setInvalidated(invalidate);
    }
}

// Add this widget to widgets
TFT_Widget *TFT_Widget::add(TFT_Widget *widget)
{
    children[child++] = widget;
    widget->setOwner(this);
    // Fix new position relative to owner x and y position
    widget->setPosition(x, y);
    return widget;
}

void TFT_Widget::setPosition(int16_t _x, int16_t _y)
{
    this->x += _x;
    this->y += _y;
    for (int index = 0; index < child; index++)
    {
        children[index]->setPosition(this->x, this->y);
    }
}

// Notify and detect invalidate state
void TFT_Widget::notify(const Event *event)
{
    for (int index = 0; index < child; index++)
    {
        children[index]->notify(event);
    }
}

// Render this widget
void TFT_Widget::render()
{
    if (visible)
    {
        for (int index = 0; index < child; index++)
        {
            children[index]->render();
        }
    }
}

void TFT_Widget::setOwner(TFT_Widget *_owner)
{
    owner = _owner;
}

void TFT_Widget::submit(Event *event)
{
    if (owner)
    {
        owner->submit(event);
    }
}

TFT_Layer::TFT_Layer(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, _x, _y, _w, _h);
}

// Constructor
TFT_Button::TFT_Button(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, _x, _y, _w, _h);
    strcpy(label, _label);
}

// Button event handler
void TFT_Button::notify(const Event *event)
{
    switch (event->type)
    {
    // Touch on screen
    case touch:
        if (event->touch.x > x && event->touch.y > y && event->touch.x < (x + w) && event->touch.y < (y + h))
        {
            state = on;
            static Event evt;
            evt.type = buttonDown;
            evt.sender = this->id;
            this->submit(&evt);
            // draw neede
            this->invalidated = true;
        }
        break;
    // Touch is released
    case release:
        state = off;
        // draw neede
        this->invalidated = true;
        break;
    default:
        break;
    }
}

void TFT_Button::render()
{
    // render children
    this->TFT_Widget::render();
    // on draw if needed
    if (this->invalidated)
    {
        this->tft->setTextFont(1);
        this->tft->setTextColor(BUTTON_TEXT);
        this->tft->setTextSize(1);
        uint8_t r = min(w, h) / 3;
        this->tft->fillRoundRect(x, y, w, h, r, state == on ? TFT_RED : TFT_GREEN);
        this->tft->drawRoundRect(x, y, w, h, r, TFT_RED);
        uint8_t tempdatum = this->tft->getTextDatum();
        this->tft->setTextDatum(MC_DATUM);
        this->tft->drawString(label, x + (w / 2), y + (h / 2));
        this->invalidated = false;
    }
}

// Constructor
TFT_Joystick::TFT_Joystick(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, _x, _y, _w, _h);
    strcpy(label, _label);

    // Init all button
    static TFT_Button xleft(_id + 1, "Left", 0, 50);
    this->add(&xleft);
    static TFT_Button xright(_id + 2, "Right", 100, 50);
    this->add(&xright);
    static TFT_Button yup(_id + 4, "Up", 50, 0);
    this->add(&yup);
    static TFT_Button ydown(_id + 3, "Down", 50, 100);
    this->add(&ydown);
    static TFT_Button zup(_id + 6, "Up", 135, 0);
    this->add(&zup);
    static TFT_Button zdown(_id + 5, "Down", 135, 100);
    this->add(&zdown);

    // Pas 0.1, 1, 10 and 100
    static TFT_Button pas(_id + 6, "Pas", 50, 50);
    this->add(&pas);
}

// Constructor
TFT_Console::TFT_Console(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, _x, _y, _w, _h);
    strcpy(label, _label);

    // Init all button
    static TFT_Label line01(_id + 1, "", 0, 0);
    this->add(&line01);
    static TFT_Label line02(_id + 1, "", 0, 13 * 1);
    this->add(&line02);
    static TFT_Label line03(_id + 1, "", 0, 13 * 2);
    this->add(&line03);
    static TFT_Label line04(_id + 1, "", 0, 13 * 3);
    this->add(&line04);
    static TFT_Label line05(_id + 1, "", 0, 13 * 4);
    this->add(&line05);
    static TFT_Label line06(_id + 1, "", 0, 13 * 5);
    this->add(&line06);

    this->lines[0] = &line01;
    this->lines[1] = &line02;
    this->lines[2] = &line03;
    this->lines[3] = &line04;
    this->lines[4] = &line05;
    this->lines[5] = &line06;
}

void TFT_Console::write(const char *message)
{
    this->lines[0]->setLabel(this->lines[1]->getLabel());
    this->lines[1]->setLabel(this->lines[2]->getLabel());
    this->lines[2]->setLabel(this->lines[3]->getLabel());
    this->lines[3]->setLabel(this->lines[4]->getLabel());
    this->lines[4]->setLabel(this->lines[5]->getLabel());
    this->lines[5]->setLabel(message);
}

// Constructor
TFT_Label::TFT_Label(int16_t _id, const char *_label, int16_t _x, int16_t _y)
{
    init(_id, _x, _y, 0, 0);
    strcpy(label, _label);
}

void TFT_Label::render()
{
    // draw children
    this->TFT_Widget::render();
    // on draw if needed
    if (this->invalidated)
    {
        this->tft->setTextFont(1);                              // use Font2 = 16 pixel X 7 probably
        this->tft->setTextSize(1);                              // char is 2 X magnified =>
        this->tft->setTextColor(SCREEN_NORMAL_TEXT, TFT_BLACK); // when only 1 parameter, background = fond);
        this->tft->setTextDatum(TL_DATUM);                      // align rigth ( option la plus pratique pour les float ou le statut GRBL)
        this->tft->drawString(label, x, y);
        this->invalidated = false;
    }
}