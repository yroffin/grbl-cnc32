#include "ui.hpp"
#include "evt-ctrl.hpp"
#include "grbl-ctrl.hpp"
#include "utils.hpp"

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

// Capture screen event
void EvtCtrl::capture()
{
    int16_t x, y;
    if (TFT_Screen::instance()->getTouch(&x, &y))
    {
        // screen is pressed
        if (!touched)
        {
            this->sendTouch(WIDGET_ID_SCREEN, TOUCH_SCREEN, x, y);
            touched = true;
            touchedTimestamp = millis();
        }
        // screen is pressed (long)
        if (touched && (millis() - touchedTimestamp) > 2000)
        {
            this->sendTouch(WIDGET_ID_SCREEN, LONG_TOUCH_SCREEN, x, y);
        }
    }
    else
    {
        // no action on screen
        if (touched && (millis() - touchedTimestamp) > 100)
        {
            // screen is unpressed after 100 ms
            this->send(WIDGET_ID_SCREEN, RELEASE_SCREEN);
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

// Phase2 (handle new event)
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

// Register a simple event
void EvtCtrl::send(int16_t sender, EventType event)
{
    eventStore[count].type = event;
    eventStore[count].timestamp = millis();
    eventStore[count].sender = sender;
    count++;
}

// Register a char* event
void EvtCtrl::sendString(int16_t sender, EventType event, const char *value)
{
    eventStore[count].type = event;
    eventStore[count].timestamp = millis();
    eventStore[count].sender = sender;
    Utils::strcpy(eventStore[count].message, value, sizeof(eventStore[count].message));
    log_i("%d %s", eventStore[count].type, eventStore[count].message);
    count++;
}

// Register a int event
void EvtCtrl::sendInt(int16_t sender, EventType event, int value)
{
    eventStore[count].type = event;
    eventStore[count].timestamp = millis();
    eventStore[count].sender = sender;
    eventStore[count].ivalue = value;
    count++;
}

// Register a int event
void EvtCtrl::sendFloat(int16_t sender, EventType event, float value)
{
    eventStore[count].type = event;
    eventStore[count].timestamp = millis();
    eventStore[count].sender = sender;
    eventStore[count].fvalue.f1 = value;
    count++;
}

// Register a int event
void EvtCtrl::sendTouch(int16_t sender, EventType event, int16_t a, int16_t b)
{
    eventStore[count].type = event;
    eventStore[count].timestamp = millis();
    eventStore[count].sender = sender;
    eventStore[count].touch.x = a;
    eventStore[count].touch.y = b;
    count++;
}

// Register a float event
void EvtCtrl::sendVector(int16_t sender, EventType event, float f1, float f2, float f3)
{
    eventStore[count].type = event;
    eventStore[count].timestamp = millis();
    eventStore[count].sender = sender;
    eventStore[count].fvalue.f1 = f1;
    eventStore[count].fvalue.f2 = f2;
    eventStore[count].fvalue.f3 = f3;
    count++;
}

// Flush all event
void EvtCtrl::flush()
{
    count = 0;
}