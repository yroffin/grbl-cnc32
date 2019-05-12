#include "evt-ctrl.hpp"
#include "config.h"

EvtCtrl::EvtCtrl(TFT_eSPI &_tft) : tft(_tft)
{
}

void EvtCtrl::init()
{
}

void EvtCtrl::capture()
{
    int16_t x, y;
    if (tft.getTouch(&x, &y, 800))
    {
        // screen is pressed
        if (!touched)
        {
            touchEvent(x, y);
            touched = true;
            touchedTimestamp = millis();
        }
    }
    else
    {
        // no action on screen
        if (touched && (millis() - touchedTimestamp) > 100)
        {
            // screen is unpressed after 100 ms
            releaseEvent();
            touched = false;
        }
    }
}

// Register a touch event
void EvtCtrl::touchEvent(int16_t _x, int16_t _y)
{
    eventStore[count].type = touch;
    eventStore[count].timestamp = millis();
    eventStore[count].touch.x = _x;
    eventStore[count].touch.y = _y;
    count++;
}

// Register a release touch event
void EvtCtrl::releaseEvent()
{
    eventStore[count].type = release;
    eventStore[count].timestamp = millis();
    count++;
}

int16_t EvtCtrl::countEvents()
{
    return count;
}

// retrieve event with given index
const Event *EvtCtrl::getEvent(int16_t index)
{
    return &eventStore[index];
}

void EvtCtrl::flush()
{
    count = 0;
}