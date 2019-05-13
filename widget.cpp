#include "evt-ctrl.hpp"
#include "widget.hpp"
#include "config.h"

TFT_Screen::TFT_Screen(TFT_eSPI &_tft, EvtCtrl &_evtCtrl) : tft(_tft), evtCtrl(_evtCtrl)
{
}

// init screen
void TFT_Screen::init()
{
    tft.init(); // Initialise l'écran avec les pins définies dans setup
    // Set the rotation before we calibrate
    tft.setRotation(1); // normally, this is already done in tft.int() but it is not clear how is rotation set (probably 0); so it can be usefull to change it here
    calibrate();        // call screen calibration
    // Init all layer
    menuLayer();
    controlLayer();
    statLayer();
    // First render
    render();
}

// menu layer
void TFT_Screen::menuLayer()
{
    // Init all layer
    static TFT_Layer layer(tft, WIDGET_ID_LAYER_MENU, 0, 0);
    add(&layer)->setVisible(true);
    static TFT_Label title(tft, WIDGET_ID_DEFAULT, "Menu", 0, 0);
    layer.add(&title);
    static TFT_Button a(tft, WIDGET_ID_LAYER_MENU_BTNA, "a", 10, 10);
    layer.add(&a);
    static TFT_Button b(tft, WIDGET_ID_LAYER_MENU_BTNB, "b", 10, 100);
    layer.add(&b);
    static TFT_Label footer(tft, WIDGET_ID_LAYER_MENU_FOOTER, "Footer", 0, 400);
    layer.add(&footer);
}

// control layer
void TFT_Screen::controlLayer()
{
    // Init all layer
    static TFT_Layer layer(tft, WIDGET_ID_LAYER_CTRL, 40, 0);
    add(&layer)->setVisible(false);
    static TFT_Label title(tft, WIDGET_ID_DEFAULT, "Control", 0, 0);
    layer.add(&title);
    static TFT_Button xp(tft, WIDGET_ID_LAYER_CTRL_BTNXM, "x+", 10, 10);
    layer.add(&xp);
    static TFT_Button xm(tft, WIDGET_ID_LAYER_CTRL_BTNXM, "x-", 10, 100);
    layer.add(&xm);
    static TFT_Button yp(tft, WIDGET_ID_LAYER_CTRL_BTNYM, "y+", 50, 10);
    layer.add(&yp);
    static TFT_Button ym(tft, WIDGET_ID_LAYER_CTRL_BTNYM, "y-", 50, 100);
    layer.add(&ym);
    static TFT_Button zp(tft, WIDGET_ID_LAYER_CTRL_BTNZM, "z+", 100, 10);
    layer.add(&zp);
    static TFT_Button zm(tft, WIDGET_ID_LAYER_CTRL_BTNZM, "z-", 100, 100);
    layer.add(&zm);
}

// stat layer
void TFT_Screen::statLayer()
{
    // Init all layer
    static TFT_Layer layer(tft, WIDGET_ID_LAYER_STAT, 100, 0);
    add(&layer)->setVisible(true);
    static TFT_Label title(tft, WIDGET_ID_DEFAULT, "Stat", 0, 0);
    layer.add(&title);
    static TFT_Label grblStatus(tft, WIDGET_ID_DEFAULT, "status", 0, 16);
    layer.add(&grblStatus);
    static TFT_Label grblStatusValue(tft, WIDGET_ID_LAYER_STAT_GRBL_STATUS, "...", 24, 16);
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
        tft.setTouch(calData);
    }
    else
    {
        // data not valid so recalibrate
        tft.fillScreen(SCREEN_BACKGROUND);
        tft.setCursor(20, 0);
        tft.setTextFont(2);
        tft.setTextSize(1);
        tft.setTextColor(SCREEN_HEADER_TEXT, SCREEN_BACKGROUND);

        tft.println(__TOUCH_CORNER);

        tft.setTextFont(1);
        tft.println();

        tft.calibrateTouch(calData, SCREEN_ALERT_TEXT, SCREEN_BACKGROUND, 15);
        tft.setTextColor(SCREEN_NORMAL_TEXT, SCREEN_BACKGROUND);
        tft.println(__CAL_COMPLETED);
        //tft.printf("Cal data %d %d %d %d %d\n", calData[0], calData[1], calData[2], calData[3], calData[4]);

        TFT_Widget::init(WIDGET_ID_LAYER_MENU, 0, 0, calData[0], calData[1]);
    }
}

// Add a new layer
TFT_Layer *TFT_Screen::add(TFT_Layer *layer)
{
    layers[count++] = layer;
    layer->setOwner(this);
    return layer;
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
}

// Update screen status
void TFT_Screen::status(const char *message)
{
    this->setLabelById(WIDGET_ID_LAYER_MENU_FOOTER, message);
}

// Notify event
bool TFT_Screen::notify(const Event *event)
{
    bool invalidate = false;

    // Handle event on screen level
    if (event->type == grblStatus)
    {
        log_i("EEEE: %s", event->message);
        this->setLabelById(WIDGET_ID_LAYER_STAT_GRBL_STATUS, event->message);
        invalidate = true;
    }

    // Dispatch to layers
    for (int index = 0; index < count; index++)
    {
        if (layers[index]->notify(event))
        {
            invalidate = true;
        }
    }
    return invalidate;
}

// Notify each event to each layer
bool TFT_Screen::isInvalidated()
{
    bool invalidate = false;
    int16_t maxEvents = evtCtrl.countEvents();
    for (int e = 0; e < maxEvents; e++)
    {
        if (notify(evtCtrl.getEvent(e)))
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
    tft.fillScreen(TFT_BLACK);
    // Display
    for (int index = 0; index < count; index++)
    {
        layers[index]->render();
    }
    // Debug
    int16_t maxEvents = evtCtrl.countEvents();
    for (int e = 0; e < maxEvents; e++)
    {
        if (evtCtrl.getEvent(e)->type == touch)
        {
            const Event *evt = evtCtrl.getEvent(e);
            tft.drawFastHLine(0, evt->touch.y, 1024, TFT_RED);
            tft.drawFastVLine(evt->touch.x, 0, 1024, TFT_RED);
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

void TFT_Widget::setLabelById(int16_t _id, const char *_label)
{
    findById(_id)->setLabel(_label);
}

void TFT_Widget::setVisibleById(int16_t _id, bool _active)
{
    findById(_id)->setVisible(_active);
}

void TFT_Widget::render()
{
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

TFT_Layer::TFT_Layer(TFT_eSPI &_tft, int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : tft(_tft)
{
    init(_id, _x, _y, _w, _h);
}

// Add this widget to widgets
TFT_Widget *TFT_Layer::add(TFT_Widget *widget)
{
    widgets[count++] = widget;
    widget->setOwner(this);
    // Fix new position relative to owner x and y position
    widget->setPosition(x, y);
    return widget;
}

// Notify and detect invalidate state
bool TFT_Layer::notify(const Event *event)
{
    bool invalidate = false;
    for (int index = 0; index < count; index++)
    {
        if (widgets[index]->notify(event))
        {
            invalidate = true;
        }
    }
    return invalidate;
}

// Render only if visible
void TFT_Layer::render()
{
    if (visible)
    {
        for (int index = 0; index < count; index++)
        {
            widgets[index]->render();
        }
    }
}

// Constructor
TFT_Button::TFT_Button(TFT_eSPI &_tft, int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h) : tft(_tft)
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
    tft.setTextFont(1);
    tft.setTextColor(BUTTON_TEXT);
    tft.setTextSize(1);
    uint8_t r = state == on ? min(w, h) / 2 : min(w, h) / 4; // Corner radius
    tft.fillRoundRect(x, y, w, h, r, BUTTON_BACKGROUND);
    tft.drawRoundRect(x, y, w, h, r, BUTTON_BORDER_NOT_PRESSED);
    uint8_t tempdatum = tft.getTextDatum();
    tft.setTextDatum(MC_DATUM);
    tft.drawString(label, x + (w / 2), y + (h / 2));
}

// Constructor
TFT_Label::TFT_Label(TFT_eSPI &_tft, int16_t _id, const char *_label, int16_t _x, int16_t _y) : tft(_tft)
{
    init(_id, _x, _y, 0, 0);
    strcpy(label, _label);
}

void TFT_Label::render()
{
    tft.setTextFont(1);                              // use Font2 = 16 pixel X 7 probably
    tft.setTextSize(1);                              // char is 2 X magnified =>
    tft.setTextColor(SCREEN_NORMAL_TEXT, TFT_BLACK); // when only 1 parameter, background = fond);
    tft.setTextDatum(TL_DATUM);                      // align rigth ( option la plus pratique pour les float ou le statut GRBL)
    tft.drawString(label, x, y);
}