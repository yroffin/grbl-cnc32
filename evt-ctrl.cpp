#include "ui.hpp"
#include "evt-ctrl.hpp"
#include "grbl-ctrl.hpp"

// Event controller
EvtCtrl *__instance_evt = 0;

// singleton
EvtCtrl *EvtCtrl::instance()
{
    if (__instance_evt == 0)
    {
        __instance_evt = new EvtCtrl();
    }
    return __instance_evt;
}

EvtCtrl::EvtCtrl()
{
}

void EvtCtrl::init()
{
}

void EvtCtrl::capture()
{
    int16_t x, y;
    if (TFT_Screen::instance()->getTouch(&x, &y))
    {
        // screen is pressed
        if (!touched)
        {
            touchEvent(WIDGET_ID_SCREEN, x, y);
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
            releaseEvent(WIDGET_ID_SCREEN);
            touched = false;
        }
    }
}

// Phase1
void EvtCtrl::dispatchPrimaryEvents()
{
    if (count == 0)
        return;
    // log_i("Dispatch phase 1 %d/%d", 0, this->primaryEvents);
    this->primaryEvents = count;
    for (int e = 0; e < this->primaryEvents; e++)
    {
        Event *event = &eventStore[e];
        // Display it
        // log_i("A[%03d] event: %d sender: %08x", e, event->type, event->sender);
        TFT_Screen::instance()->notify(event);
        GrblCtrl::instance()->notify(event);
    }
}

// Phase2
boolean EvtCtrl::dispatchSecondaryEvents()
{
    if (this->primaryEvents >= count)
        return false;
    // log_i("Dispatch phase 2 %d/%d", this->primaryEvents, count);
    for (int e = this->primaryEvents; e < count; e++)
    {
        Event *event = &eventStore[e];
        // Display it
        // log_i("B[%03d] phase2 event: %d sender: %08x", e, event->type, event->sender);
        TFT_Screen::instance()->notify(event);
        GrblCtrl::instance()->notify(event);
    }
    this->primaryEvents = count;
    return (this->primaryEvents >= count);
}

// Register a touch event
void EvtCtrl::touchEvent(int16_t sender, int16_t _x, int16_t _y)
{
    eventStore[count].type = touch;
    eventStore[count].timestamp = millis();
    eventStore[count].touch.x = _x;
    eventStore[count].touch.y = _y;
    eventStore[count].sender = sender;
    count++;
}

// Register a release touch event
void EvtCtrl::releaseEvent(int16_t sender)
{
    eventStore[count].type = release;
    eventStore[count].timestamp = millis();
    eventStore[count].sender = sender;
    count++;
}

// Register a simple event
void EvtCtrl::send(int16_t sender, EventType event)
{
    eventStore[count].type = event;
    eventStore[count].timestamp = millis();
    eventStore[count].sender = sender;
    count++;
}

// Register a char* event
void EvtCtrl::sendWithString(int16_t sender, EventType event, const char *value)
{
    eventStore[count].type = event;
    eventStore[count].timestamp = millis();
    eventStore[count].sender = sender;
    strcpy(eventStore[count].message, value);
    count++;
}

// Register a float event
void EvtCtrl::sendWithFloat(int16_t sender, EventType event, float value)
{
    eventStore[count].type = event;
    eventStore[count].timestamp = millis();
    eventStore[count].sender = sender;
    eventStore[count].fvalue.f1 = value;
    count++;
}

// Register a float event
void EvtCtrl::sendWithVector(int16_t sender, EventType event, float f1, float f2, float f3)
{
    eventStore[count].type = event;
    eventStore[count].timestamp = millis();
    eventStore[count].sender = sender;
    eventStore[count].fvalue.f1 = f1;
    eventStore[count].fvalue.f2 = f2;
    eventStore[count].fvalue.f3 = f3;
    count++;
}

// show layer
void EvtCtrl::showEvent(int16_t sender, int16_t target)
{
    eventStore[count].type = showComponent;
    eventStore[count].timestamp = millis();
    eventStore[count].sender = sender;
    eventStore[count].target = target;
    count++;
}

// hide layer
void EvtCtrl::hideEvent(int16_t sender, int16_t target)
{
    eventStore[count].type = hideComponent;
    eventStore[count].timestamp = millis();
    eventStore[count].sender = sender;
    eventStore[count].target = target;
    count++;
}

void EvtCtrl::flush()
{
    count = 0;
}