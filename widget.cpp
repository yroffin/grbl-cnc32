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
    boolean change = this->visible != _active;
    if (!change)
        return;
    this->visible = _active;

    // update all children
    for (int index = 0; index < child; index++)
    {
        children[index]->setVisible(this->visible);
    }

    if (this->visible)
    {
        // invalidate all children too
        this->setInvalidated(true);
        EvtCtrl::instance()->showEvent(this->id, this->id);
    }
    else
    {
        EvtCtrl::instance()->hideEvent(this->id, this->id);
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
    if (visible)
    {
        for (int index = 0; index < child; index++)
        {
            children[index]->render();
        }
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
        if (this->isVisible() && event->touch.x > x && event->touch.y > y && event->touch.x < (x + w) && event->touch.y < (y + h))
        {
            state = on;
            EvtCtrl::instance()->buttonDownEvent(this->id);
            // draw neede
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

void TFT_Button::render()
{
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
    // render children
    this->TFT_Widget::render();
}

// Constructor
TFT_File::TFT_File(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
    : TFT_Button(_id, _label, _x, _y, _w, _h)
{
}

void TFT_File::render()
{
    // on draw if needed
    if (this->invalidated)
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
        this->invalidated = false;
    }
    // render children
    this->TFT_Widget::render();
}

// Constructor
TFT_Joystick::TFT_Joystick(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, _x, _y, _w, _h);
    strcpy(label, _label);

    // Init all button
    this->xleft = new TFT_Button(_id + 1, "Left", 0, 50);
    this->add(this->xleft);
    this->xright = new TFT_Button(_id + 2, "Right", 100, 50);
    this->add(this->xright);
    this->yup = new TFT_Button(_id + 4, "Up", 50, 0);
    this->add(this->yup);
    this->ydown = new TFT_Button(_id + 3, "Down", 50, 100);
    this->add(this->ydown);
    this->zup = new TFT_Button(_id + 6, "Up", 135, 0);
    this->add(this->zup);
    this->zdown = new TFT_Button(_id + 5, "Down", 135, 100);
    this->add(this->zdown);

    // Pas 0.1, 1, 10 and 100
    this->pas = new TFT_Button(_id + 6, "Pas", 50, 50);
    this->add(this->pas);
}

// Constructor
TFT_Console::TFT_Console(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, _x, _y, _w, _h);
    strcpy(label, _label);

    this->lines[0] = new TFT_Label(_id, "", 0, 0);
    this->add(this->lines[0]);
    this->lines[1] = new TFT_Label(_id, "", 0, 13 * 1);
    this->add(this->lines[1]);
    this->lines[2] = new TFT_Label(_id, "", 0, 13 * 2);
    this->add(this->lines[2]);
    this->lines[3] = new TFT_Label(_id, "", 0, 13 * 3);
    this->add(this->lines[3]);
    this->lines[4] = new TFT_Label(_id, "", 0, 13 * 4);
    this->add(this->lines[4]);
    this->lines[5] = new TFT_Label(_id, "", 0, 13 * 5);
    this->add(this->lines[5]);
}

void TFT_Console::write(const char *message)
{
    this->lines[0]->setLabel(this->lines[1]->getLabel());
    this->lines[1]->setLabel(this->lines[2]->getLabel());
    this->lines[2]->setLabel(this->lines[3]->getLabel());
    this->lines[3]->setLabel(this->lines[4]->getLabel());
    this->lines[4]->setLabel(this->lines[5]->getLabel());
    this->lines[5]->setLabel(message);
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
    if (event->type == buttonDown && event->sender == this->id)
    {
        this->onLeft();
        EvtCtrl::instance()->fileGridEvent(this->id);
        this->invalidated = true;
    }
    if (event->type == buttonDown && event->sender == this->id + 1)
    {
        this->onRight();
        EvtCtrl::instance()->fileGridEvent(this->id);
        this->invalidated = true;
    }
    // select line
    for (int l = 1; l < 4; l++)
    {
        if (event->type == buttonDown && event->sender == this->id + 2 + l)
        {
            EvtCtrl::instance()->fileGridSelect(this->id + 2 + l, this->lines[l]->getLabel());
        }
    }
    // Dispatch to layers
    this->TFT_Widget::notify(event);
}

void TFT_FileGrid::render()
{
    // on draw if needed
    if (this->invalidated)
    {
        log_i("r %d,%d %d,%d", x, y, w, h);
        this->tft->fillRect(x, y, w, h, TFT_BLACK);
        // render children
        this->TFT_Widget::render();
        this->invalidated = false;
    }
    // render children
    this->TFT_Widget::render();
}

// Constructor
TFT_Label::TFT_Label(int16_t _id, const char *_label, int16_t _x, int16_t _y)
{
    init(_id, _x, _y, 0, 0);
    strcpy(label, _label);
}

void TFT_Label::render()
{
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
    // draw children
    this->TFT_Widget::render();
}