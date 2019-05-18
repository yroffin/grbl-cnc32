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
    static TFT_Button a(WIDGET_ID_LAYER_MENU_BTNA, "a", 10, 10);
    layer.add(&a);
    static TFT_Button b(WIDGET_ID_LAYER_MENU_BTNB, "b", 10, 100);
    layer.add(&b);
    static TFT_Label footer(WIDGET_ID_LAYER_MENU_FOOTER, "......", 0, 210);
    layer.add(&footer);
}

// control layer
void TFT_Screen::controlLayer()
{
    log_i("TFT_Screen control layer ...");
    // Init all layer
    static TFT_Layer layer(WIDGET_ID_LAYER_CTRL, 60, 0);
    add(&layer)->setVisible(false);
    static TFT_Label title(WIDGET_ID_DEFAULT, "Control", 0, 0);
    layer.add(&title);
    static TFT_Joystick joystick(WIDGET_ID_LAYER_CTRL_JOYSTICK, "Control", 0, 20);
    layer.add(&joystick);
}

// stat layer
void TFT_Screen::statLayer()
{
    log_i("TFT_Screen statistics layer ...");
    // Init all layer
    static TFT_Layer layer(WIDGET_ID_LAYER_STAT, 100, 0);
    add(&layer)->setVisible(true);
    static TFT_Label title(WIDGET_ID_DEFAULT, "Stat", 0, 0);
    layer.add(&title);
    static TFT_Label grblStatus(WIDGET_ID_DEFAULT, "status", 0, 16);
    layer.add(&grblStatus);
    static TFT_Label grblStatusValue(WIDGET_ID_LAYER_STAT_GRBL_STATUS, "...", 24, 16);
    layer.add(&grblStatusValue);
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
        this->setVisibleById(WIDGET_ID_LAYER_CTRL, true);
        this->setVisibleById(WIDGET_ID_LAYER_STAT, false);
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_MENU_BTNB)
    {
        this->setVisibleById(WIDGET_ID_LAYER_CTRL, false);
        this->setVisibleById(WIDGET_ID_LAYER_STAT, true);
    }
    // Submit event to other controller
    GrblCtrl::instance()->submit(event);
    // Display it
    this->status("event: %d sender: %08x", event->type, event->sender);
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
}

// Notify event
bool TFT_Screen::notify(const Event *event)
{
    bool invalidate = false;

    // Handle event on screen level
    if (event->type == grblStatus)
    {
        this->setLabelById(WIDGET_ID_LAYER_STAT_GRBL_STATUS, event->message);
        invalidate = true;
    }

    // Dispatch to layers
    return this->TFT_Widget::notify(event) || invalidate;
}

// Notify each event to each layer
bool TFT_Screen::isInvalidated()
{
    bool invalidate = false;
    int16_t maxEvents = EvtCtrl::instance()->countEvents();
    for (int e = 0; e < maxEvents; e++)
    {
        if (notify(EvtCtrl::instance()->getEvent(e)))
        {
            invalidate = true;
        }
    }
    return invalidate;
}

// Render
void TFT_Screen::render()
{
    // Clear
    this->tft->fillScreen(TFT_BLACK);
    // Display
    this->TFT_Widget::render();
    // Event
    int16_t maxEvents = EvtCtrl::instance()->countEvents();
    for (int e = 0; e < maxEvents; e++)
    {
        if (EvtCtrl::instance()->getEvent(e)->type == touch)
        {
            const Event *evt = EvtCtrl::instance()->getEvent(e);
            this->tft->drawFastHLine(0, evt->touch.y, 1024, TFT_RED);
            this->tft->drawFastVLine(evt->touch.x, 0, 1024, TFT_RED);
        }
    }
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
}

void TFT_Widget::setVisibleById(int16_t _id, bool _active)
{
    findById(_id)->setVisible(_active);
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
bool TFT_Widget::notify(const Event *event)
{
    bool invalidate = false;
    for (int index = 0; index < child; index++)
    {
        if (children[index]->notify(event))
        {
            invalidate = true;
        }
    }
    return invalidate;
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
bool TFT_Button::notify(const Event *event)
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
            submit(&evt);
            return true;
        }
        break;
    // Touch is released
    case release:
        state = off;
        return true;
        break;
    default:
        break;
    }
    return false;
}

void TFT_Button::render()
{
    this->tft->setTextFont(1);
    this->tft->setTextColor(BUTTON_TEXT);
    this->tft->setTextSize(1);
    uint8_t r = state == on ? min(w, h) / 2 : min(w, h) / 4; // Corner radius
    this->tft->fillRoundRect(x, y, w, h, r, BUTTON_BACKGROUND);
    this->tft->drawRoundRect(x, y, w, h, r, BUTTON_BORDER_NOT_PRESSED);
    uint8_t tempdatum = this->tft->getTextDatum();
    this->tft->setTextDatum(MC_DATUM);
    this->tft->drawString(label, x + (w / 2), y + (h / 2));
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
    static TFT_Button zup(_id + 6, "Up", 150, 0);
    this->add(&zup);
    static TFT_Button zdown(_id + 5, "Down", 150, 100);
    this->add(&zdown);
}

// Constructor
TFT_Label::TFT_Label(int16_t _id, const char *_label, int16_t _x, int16_t _y)
{
    init(_id, _x, _y, 0, 0);
    strcpy(label, _label);
}

void TFT_Label::render()
{
    this->tft->setTextFont(1);                              // use Font2 = 16 pixel X 7 probably
    this->tft->setTextSize(1);                              // char is 2 X magnified =>
    this->tft->setTextColor(SCREEN_NORMAL_TEXT, TFT_BLACK); // when only 1 parameter, background = fond);
    this->tft->setTextDatum(TL_DATUM);                      // align rigth ( option la plus pratique pour les float ou le statut GRBL)
    this->tft->drawString(label, x, y);
}