#include "widget.hpp"
#include "evt-ctrl.hpp"
#include "storage-ctrl.hpp"

// Global for internal use
TFT_Widget *__registry[128];
int8_t __registryCount = 0;
TFT_eSPI _tft = TFT_eSPI();

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

void TFT_Widget::setLabel(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vsprintf(this->label, format, args);
    va_end(args);
    // draw needed
    this->invalidated = true;
}

void TFT_Widget::show()
{
    this->visible = true;

    // update all children
    for (int index = 0; index < child; index++)
    {
        children[index]->show();
    }

    // invalidate all children to force draw
    this->invalidated = true;
    EvtCtrl::instance()->showEvent(this->id, this->id);
}

void TFT_Widget::hide()
{
    this->visible = false;

    // update all children
    for (int index = 0; index < child; index++)
    {
        children[index]->hide();
    }
    EvtCtrl::instance()->hideEvent(this->id, this->id);
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
        children[index]->setPosition(_x, _y);
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
    if (this->visible)
    {
        if (this->invalidated)
        {
            this->draw();
        }
    }
    for (int index = 0; index < child; index++)
    {
        children[index]->render();
    }
}

// Render this widget
void TFT_Widget::draw()
{
    this->tft->fillRect(x, y, w, h, this->background);
    //this->tft->drawRect(x, y, w, h, TFT_DARKGREEN);
}

// Constructor
TFT_Layer::TFT_Layer(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, _x, _y, _w, _h);
}

// Constructor
TFT_Button::TFT_Button(int16_t _id, const char *_label, int16_t _x, int16_t _y)
{
    init(_id, _x, _y, 40, 40);
    strcpy(label, _label);
}

// Change emitted event
void TFT_Button::setEvent(EventType event)
{
    this->eventType = event;
}

// Button event handler
void TFT_Button::notify(const Event *event)
{
    switch (event->type)
    {
    // Touch on screen
    case touch:
        if (this->visible && event->touch.x > x && event->touch.y > y && event->touch.x < (x + w) && event->touch.y < (y + h))
        {
            EvtCtrl::instance()->send(this->id, this->eventType);
            state = on;
            // draw needed
            this->invalidated = true;
        }
        break;
    // Touch is released
    case release:
        state = off;
        // draw needed
        this->invalidated = true;
        break;
    default:
        break;
    }
}

void TFT_Button::draw()
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
}

// Constructor
TFT_File::TFT_File(int16_t _id, const char *_label, int16_t _x, int16_t _y)
    : TFT_Button(_id, _label, _x, _y)
{
}

void TFT_File::draw()
{
    this->tft->setTextFont(1);
    this->tft->setTextColor(TFT_DARKGREY);
    this->tft->setTextSize(1);
    uint8_t r = min(w, h) / 3;
    this->tft->fillRoundRect(x, y, w, h, r, state == on ? TFT_RED : TFT_GREEN);
    this->tft->drawRoundRect(x, y, w, h, r, TFT_RED);
    this->tft->setTextDatum(TL_DATUM);
    this->tft->drawString("x", x + (w / 2), y + (h / 2));
    this->tft->drawString(label, x + (w / 2) + 24, y + (h / 2));
}

// Constructor
TFT_Joystick::TFT_Joystick(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, _x, _y, _w, _h);
    strcpy(label, _label);

    // Init all button
    this->xleft = new TFT_Button(_id + 1, "Left", 0, 44);
    this->add(this->xleft);
    this->xright = new TFT_Button(_id + 2, "Right", 44 * 2, 44);
    this->add(this->xright);
    this->yup = new TFT_Button(_id + 4, "Up", 44, 0);
    this->add(this->yup);
    this->ydown = new TFT_Button(_id + 3, "Down", 44, 44 * 2);
    this->add(this->ydown);
    this->zup = new TFT_Button(_id + 6, "Up", 44 * 4, 0);
    this->add(this->zup);
    this->zdown = new TFT_Button(_id + 5, "Down", 44 * 4, 44 * 2);
    this->add(this->zdown);

    // Pas 0.1, 1, 10 and 100
    this->pas = new TFT_Button(_id + 7, "Pas", 44, 44);
    this->add(this->pas);
}

// Notify
void TFT_Joystick::notify(const Event *event)
{
    if (event->type == EVENT_NEW_STEP)
    {
        this->pas->setLabel("%03.0f", event->fvalue.f1);
    }
    if (event->type == BUTTON_DOWN)
    {
        if (event->sender == this->id + 1)
        {
            EvtCtrl::instance()->send(this->id, EVENT_XM);
        }
        if (event->sender == this->id + 2)
        {
            EvtCtrl::instance()->send(this->id, EVENT_XP);
        }
        if (event->sender == this->id + 4)
        {
            EvtCtrl::instance()->send(this->id, EVENT_YP);
        }
        if (event->sender == this->id + 3)
        {
            EvtCtrl::instance()->send(this->id, EVENT_YM);
        }
        if (event->sender == this->id + 6)
        {
            EvtCtrl::instance()->send(this->id, EVENT_ZP);
        }
        if (event->sender == this->id + 5)
        {
            EvtCtrl::instance()->send(this->id, EVENT_ZM);
        }
        if (event->sender == this->id + 7)
        {
            EvtCtrl::instance()->send(this->id, EVENT_NEXT_STEP);
        }
    }
    // Dispatch to layers
    this->TFT_Widget::notify(event);
}

// Constructor
TFT_Console::TFT_Console(int16_t _id, const char *_label, int16_t _sz, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, _x, _y, _w, _h);
    strcpy(label, _label);

    this->sz = _sz;
    for (int i = 0; i < this->sz; i++)
    {
        this->lines[i] = new TFT_Label(_id, "", 0, 13 * i);
        this->add(this->lines[i]);
    }
}

void TFT_Console::write(const char *message)
{
    int i = 0;
    for (; i < this->sz - 1; i++)
    {
        this->lines[i]->setLabel(this->lines[i + 1]->getLabel());
    }
    this->lines[i]->setLabel(message);
    this->invalidated = true;
}

// Constructor
TFT_FileGrid::TFT_FileGrid(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, _x, _y, _w, _h);
    strcpy(label, _label);

    this->left = new TFT_Button(_id, "<<", 0, 0);
    this->add(this->left);
    this->right = new TFT_Button(_id + 1, ">>", 200, 0);
    this->add(this->right);

    // add lines
    for (int l = 0; l < 4; l++)
    {
        this->lines[l] = new TFT_File(_id + 2 + l, ".", 40, 40 * l);
        this->add(this->lines[l]);
    }
}

void TFT_FileGrid::clear()
{
    this->lines[0]->setLabel(".");
    this->lines[1]->setLabel(".");
    this->lines[2]->setLabel(".");
    this->lines[3]->setLabel(".");
}

void TFT_FileGrid::set(int16_t index, const char *label)
{
    this->lines[index]->setLabel(label);
    this->invalidated = true;
}

void TFT_FileGrid::notify(const Event *event)
{
    if (event->type == BUTTON_DOWN && event->sender == this->id)
    {
        this->onLeft();
        EvtCtrl::instance()->fileGridEvent(this->id);
        this->invalidated = true;
    }
    if (event->type == BUTTON_DOWN && event->sender == this->id + 1)
    {
        this->onRight();
        EvtCtrl::instance()->fileGridEvent(this->id);
        this->invalidated = true;
    }
    // select line
    for (int l = 1; l < 4; l++)
    {
        if (event->type == BUTTON_DOWN && event->sender == this->id + 2 + l)
        {
            EvtCtrl::instance()->fileGridSelect(this->id + 2 + l, this->lines[l]->getLabel());
        }
    }
    // Dispatch to others
    this->TFT_Widget::notify(event);
}

// Constructor
TFT_Label::TFT_Label(int16_t _id, const char *_label, int16_t _x, int16_t _y)
{
    init(_id, _x, _y, 0, 0);
    strcpy(label, _label);
}

void TFT_Label::draw()
{
    this->tft->setTextFont(1);                              // use Font2 = 16 pixel X 7 probably
    this->tft->setTextSize(1);                              // char is 2 X magnified =>
    this->tft->setTextColor(SCREEN_NORMAL_TEXT, TFT_BLACK); // when only 1 parameter, background = fond);
    this->tft->setTextDatum(TL_DATUM);                      // align rigth ( option la plus pratique pour les float ou le statut GRBL)
    this->tft->drawString(label, x, y);
}

// Constructor
TFT_Group::TFT_Group(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, _x, _y, _w, _h);
    strcpy(label, _label);
}
