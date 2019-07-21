#include "widget.hpp"
#include "evt-ctrl.hpp"
#include "grbl-ctrl.hpp"
#include "storage-ctrl.hpp"
#include "i18n-ctrl.hpp"
#include "utils.hpp"

// Global for internal use
TFT_eSPI _tft = TFT_eSPI();

// constructor
TFT_Widget::TFT_Widget()
{
    this->i18n = I18nCtrl::instance();
    this->config = JsonConfigCtrl::instance();
    this->background = this->getKeyAsInt("widget", "background", TFT_BLACK);
    this->selectedBackground = this->getKeyAsInt("widget", "selectedBackground", TFT_WHITE);
    this->fontNormal = this->getKeyAsInt("widget", "fontNormal", TFT_WHITE);
    this->fontSelected = this->getKeyAsInt("widget", "fontSelected", TFT_BLACK);
    this->border = this->getKeyAsInt("widget", "border", TFT_DARKGREY);
}

// Init this widget
void TFT_Widget::init(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    this->x = _x;
    this->y = _y;
    this->w = _w;
    this->h = _h;
    sprintf(this->name, "widget-%08x", _id);
    if (strlen(this->name) > 32)
    {
        log_e("internal error, name buffer overflow ...");
    }
    this->id = _id;
    Utils::strcpy(label, _label, MAXSIZE_OF_LABEL);
    this->visible = true;
    this->tft = &_tft;
}

const char *TFT_Widget::getKey(const char *k1, const char *k2)
{
    return this->i18n->getKey(k1, k2);
}

int TFT_Widget::getKeyAsInt(const char *k1, const char *k2, int def)
{
    if (this->config == 0)
    {
        return def;
    }
    return this->config->getAsInt(k1, k2, def);
}

void TFT_Widget::setLabel(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    // use utils buffer to protect memory
    vsprintf(Utils::vsprintfBuffer(), format, args);
    Utils::strcpy(this->label, Utils::vsprintfBuffer(), MAXSIZE_OF_LABEL);
    va_end(args);
    // draw needed
    this->invalidated = true;
}

void TFT_Widget::show()
{
    // set visible
    this->visible = true;

    // invalidate all children to force draw
    this->setInvalidated(true);

    // update all children
    for (int index = 0; index < child; index++)
    {
        children[index]->show();
    }
}

void TFT_Widget::hide()
{
    // hide
    this->visible = false;

    // update all children
    for (int index = 0; index < child; index++)
    {
        children[index]->hide();
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
    if (this->drawContour)
    {
        this->tft->drawRect(x, y, w, h, TFT_DARKGREY);
    }
}

// Constructor
TFT_Layer::TFT_Layer(int16_t _id, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, "", _x, _y, _w, _h);
}

// Constructor
TFT_Button::TFT_Button(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, _label, _x, _y, _w, _h);
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
    case TOUCH_SCREEN:
        if (this->visible && event->touch.x > x && event->touch.y > y && event->touch.x < (x + w) && event->touch.y < (y + h))
        {
            EvtCtrl::instance()->send(this->id, this->eventType);
            state = on;
            // draw needed
            this->invalidated = true;
        }
        break;
    // Touch on screen (long)
    case LONG_TOUCH_SCREEN:
        if (this->visible && event->touch.x > x && event->touch.y > y && event->touch.x < (x + w) && event->touch.y < (y + h))
        {
            EvtCtrl::instance()->send(this->id, this->eventType);
            state = on;
            // draw needed
            this->invalidated = true;
        }
        break;
    // Touch is released
    case RELEASE_SCREEN:
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
    this->tft->setTextColor(state == on ? this->fontSelected : this->fontNormal);
    this->tft->setTextSize(1);
    uint8_t r = min(w, h) / 3;
    this->tft->fillRoundRect(x, y, w, h, r, state == on ? this->selectedBackground : this->background);
    this->tft->drawRoundRect(x, y, w, h, r, this->border);
    this->tft->setTextDatum(MC_DATUM);
    this->tft->drawString(label, x + (w / 2), y + (h / 2));
}

// Constructor
TFT_ButtonCmd::TFT_ButtonCmd(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
    : TFT_Button(_id, _label, _x, _y, _w, _h)
{
}

// Button event handler
void TFT_ButtonCmd::notify(const Event *event)
{
    switch (event->type)
    {
    // Touch on screen
    case TOUCH_SCREEN:
        if (this->visible && event->touch.x > x && event->touch.y > y && event->touch.x < (x + w) && event->touch.y < (y + h))
        {
            TFT_Screen::instance()->dialog->show(
                PRINT_CMD,
                this->getLabel(),
                I18nCtrl::instance()->translate(I18N_STD, this->getKey("Files", "PRC"), this->getLabel()));
            state = on;
            // draw needed
            this->invalidated = true;
        }
        break;
    // Touch is released
    case RELEASE_SCREEN:
        state = off;
        // draw needed
        this->invalidated = true;
        break;
    default:
        break;
    }
}

// Constructor
TFT_ButtonJog::TFT_ButtonJog(int16_t _id, const char *_label, JOG_WAY _jw, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
    : TFT_Button(_id, _label, _x, _y, _w, _h)
{
    this->jw = _jw;
    this->xyzJogPas = this->config->getAsFloat("sys", "speed", "l1", 0.1);
}

// Button event handler
void TFT_ButtonJog::notify(const Event *event)
{
    this->TFT_Button::notify(event);
    switch (event->type)
    {
    // Touch on screen (long)
    case EVENT_NEW_STEP:
        this->xyzJogPas = event->fvalue.f1;
        break;
    // Touch on screen (long)
    case TOUCH_SCREEN:
    case LONG_TOUCH_SCREEN:
        if (this->visible && event->touch.x > x && event->touch.y > y && event->touch.x < (x + w) && event->touch.y < (y + h))
        {
            switch (this->jw)
            {
            case JOG_XP:
                GrblCtrl::instance()->jogMoveXY(this->xyzJogPas, 0.);
                break;
            case JOG_XM:
                GrblCtrl::instance()->jogMoveXY(-this->xyzJogPas, 0.);
                break;
            case JOG_YP:
                GrblCtrl::instance()->jogMoveXY(0., this->xyzJogPas);
                break;
            case JOG_YM:
                GrblCtrl::instance()->jogMoveXY(0., -this->xyzJogPas);
                break;
            case JOG_ZP:
                GrblCtrl::instance()->jogMoveZ(this->xyzJogPas);
                break;
            case JOG_ZM:
                GrblCtrl::instance()->jogMoveZ(-this->xyzJogPas);
                break;
            }
        }
        break;
    default:
        break;
    }
}

// Constructor
TFT_File::TFT_File(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h, boolean _align)
    : TFT_Button(_id, _label, _x, _y, _w, _h)
{
    this->align = _align;
}

void TFT_File::draw()
{
    // clear
    this->tft->fillRect(x, y, w, h, state == on ? this->selectedBackground : this->background);
    this->tft->setTextFont(1);
    this->tft->setTextColor(state == on ? this->fontSelected : this->fontNormal);
    this->tft->setTextSize(1);
    this->tft->setTextDatum(TL_DATUM);
    this->tft->drawString(label, x + 5, y + (h / 2));
    this->tft->drawRect(x, y, w, h, this->border);
}

// Constructor
TFT_Joystick::TFT_Joystick(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, _label, _x, _y, _w, _h);

    // Init all button
    this->xleft = new TFT_ButtonJog(WIDGET_ID_DEFAULT, this->getKey("Joystick", "XM"), JOG_XM, 0, 44, 40, 40);
    this->add(this->xleft);
    this->xright = new TFT_ButtonJog(WIDGET_ID_DEFAULT, this->getKey("Joystick", "XP"), JOG_XP, 44 * 2, 44, 40, 40);
    this->add(this->xright);
    this->yup = new TFT_ButtonJog(WIDGET_ID_DEFAULT, this->getKey("Joystick", "YP"), JOG_YP, 44, 0, 40, 40);
    this->add(this->yup);
    this->ydown = new TFT_ButtonJog(WIDGET_ID_DEFAULT, this->getKey("Joystick", "YM"), JOG_YM, 44, 44 * 2, 40, 40);
    this->add(this->ydown);
    this->zup = new TFT_ButtonJog(WIDGET_ID_DEFAULT, this->getKey("Joystick", "ZP"), JOG_ZP, 44 * 4, 0, 40, 40);
    this->add(this->zup);
    this->zdown = new TFT_ButtonJog(WIDGET_ID_DEFAULT, this->getKey("Joystick", "ZM"), JOG_ZM, 44 * 4, 44 * 2, 40, 40);
    this->add(this->zdown);

    // Step 0.1, 1, 10 and 100
    this->pas = new TFT_Button(_id + 1, this->getKey("Joystick", "P"), 44, 44, 40, 40);
    this->add(this->pas);
}

// Notify
void TFT_Joystick::notify(const Event *event)
{
    if (event->type == EVENT_NEW_STEP)
    {
        this->pas->setLabel("%02.02f", event->fvalue.f1);
    }
    if (event->type == BUTTON_DOWN)
    {
        if (event->sender == this->id + 1)
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
    init(_id, _label, _x, _y, _w, _h);

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
    init(_id, _label, _x, _y, _w, _h);

    this->left = new TFT_Button(_id, "^", 205, 0, 30, 30);
    this->left->setEvent(PREV_FILE);
    this->add(this->left);
    this->right = new TFT_Button(_id + 1, "v", 205, 30 * 4, 30, 30);
    this->right->setEvent(NEXT_FILE);
    this->add(this->right);

    // add lines
    for (int l = 0; l < this->maxLines; l++)
    {
        if (l % 2 == 0)
        {
            this->lines[l] = new TFT_File(_id + 2 + l, ".", 0, 30 * l, 200, 30, true);
        }
        else
        {
            this->lines[l] = new TFT_File(_id + 2 + l, ".", 0, 30 * l, 200, 30, false);
        }
        this->lines[l]->setEvent(SELECT_FILE);
        this->add(this->lines[l]);
    }
}

void TFT_FileGrid::clear()
{
    for (int l = 0; l < this->maxLines; l++)
    {
        this->lines[l]->setLabel("");
    }
}

void TFT_FileGrid::set(int16_t index, const char *label)
{
    this->lines[index]->setLabel(label);
    this->invalidated = true;
}

void TFT_FileGrid::notify(const Event *event)
{
    if (event->type == PREV_FILE)
    {
        this->onLeft();
        EvtCtrl::instance()->send(this->id, GRID_CHANGE);
        this->invalidated = true;
    }
    if (event->type == NEXT_FILE)
    {
        this->onRight();
        EvtCtrl::instance()->send(this->id, GRID_CHANGE);
        this->invalidated = true;
    }
    // select line
    for (int l = 0; l < this->maxLines; l++)
    {
        if (event->type == SELECT_FILE && event->sender == this->id + 2 + l)
        {
            if( StorageCtrl::instance()->isFiles()) {
                EvtCtrl::instance()->sendString(this->id + 2 + l, FILE_SELECTED, this->lines[l]->getLabel());
            }
        }
    }
    // Dispatch to others
    this->TFT_Widget::notify(event);
}

// Constructor
TFT_Label::TFT_Label(int16_t _id, const char *_label, int16_t _x, int16_t _y)
{
    init(_id, _label, _x, _y, 0, 0);
}

void TFT_Label::draw()
{
    this->tft->setTextSize(1);                                   // char is 2 X magnified =>
    this->tft->setTextColor(this->fontNormal, this->background); // when only 1 parameter, background = fond);
    this->tft->setTextDatum(TL_DATUM);                           // align rigth ( option la plus pratique pour les float ou le statut GRBL)
    this->tft->drawString(label, x, y, 1);
}

// Constructor
TFT_StatusBar::TFT_StatusBar(int16_t _id, int16_t _x, int16_t _y) : TFT_Widget()
{
    init(_id, "status", _x, _y, 320, 10);
    Utils::strcpy(this->wifiStatus, "...", 20);

    this->busyStateFgColor = this->getKeyAsInt("status", "busyStateFgColor", TFT_BLACK);
    this->busyStateBgColor = this->getKeyAsInt("status", "busyStateBgColor", TFT_GREEN);
    this->busyStateBgColorWarn = this->getKeyAsInt("status", "busyStateBgColorWarn", TFT_ORANGE);
    this->writeStatusFgColor = this->getKeyAsInt("status", "writeStatusFgColor", TFT_BLACK);
    this->writeStatusBgColor = this->getKeyAsInt("status", "writeStatusBgColor", TFT_GREEN);
    this->writeStatusBgColorWarn = this->getKeyAsInt("status", "writeStatusBgColorWarn", TFT_ORANGE);
}

void TFT_StatusBar::draw()
{
    this->tft->setTextFont(1);
    this->tft->setTextSize(1);
    this->tft->setTextDatum(TL_DATUM);
    this->tft->drawRect(x, y, w, h, TFT_DARKGREY);

    // green for ok
    // orange for busy
    // red for long busy state
    this->tft->setTextColor(this->busyStateFgColor, this->busyState ? this->busyStateBgColorWarn : this->busyStateBgColor);
    this->tft->drawChar('B', x + 1, y + 1);
    // write status
    this->tft->setTextColor(this->writeStatusFgColor, millis() - this->lastBusyWrite > 10000 ? this->writeStatusBgColorWarn : this->writeStatusBgColor);
    this->tft->drawChar(this->writeStatus[this->write % 4], x + 1 + 8, y + 1);
    // speed write
    char work[32];
    sprintf(work, "%-05.05d B/S", this->writeSpeed);
    this->tft->drawString(work, x + 1 + (8 * 2), y + 1, 1);
    // wifi status
    sprintf(work, "%-15.15s", this->wifiStatus);
    this->tft->drawString(work, x + 1 + (8 * 9), y + 1, 1);
    // print status
    int ratio = this->toPrint == 0 ? 0 : this->printed * 100 / this->toPrint;
    sprintf(work, "%-03.03d %c %-06.06d/%-06.06d", ratio, '%', this->printed, this->toPrint);
    this->tft->drawString(work, x + 1 + (8 * 21), y + 1, 1);
    // semver
    sprintf(work, "%-05.05s", SEMVER);
    this->tft->drawString(work, x + 1 + (9 * 32), y + 1, 1);
}

void TFT_StatusBar::notifyWrite(uint16_t sz)
{
    if ((millis() - this->lastWrite) > 10000)
    {
        this->writeSpeed = this->lastWriteSize / 10;
        this->lastWriteSize = 0;
        this->lastWrite = millis();
    }
    this->lastWriteSize += sz;
    this->lastBusyWrite = millis();
    this->write += sz;
    this->invalidated = true;
}

void TFT_StatusBar::notifyBusy(boolean _busyState)
{
    this->busyState = _busyState;
    this->invalidated = true;
}

void TFT_StatusBar::notifyWifiStatus(const char *status)
{
    Utils::strcpy(this->wifiStatus, status, 20);
    this->invalidated = true;
}

void TFT_StatusBar::notifyPrintStatus(boolean _isPrinting, int _printed, int _toPrint)
{
    this->isPrinting = _isPrinting;
    this->printed = _printed;
    this->toPrint = _toPrint;
    this->invalidated = true;
}

// Constructor
TFT_Group::TFT_Group(int16_t _id, const char *_label, int16_t _x, int16_t _y, int16_t _w, int16_t _h)
{
    init(_id, _label, _x, _y, _w, _h);
}
