#include "grbl-ctrl.hpp"

// GRBL controller
GrblCtrl *__instance_grbl = 0;

// singleton
GrblCtrl *GrblCtrl::instance()
{
    if (__instance_grbl == 0)
    {
        __instance_grbl = new GrblCtrl();
    }
    return __instance_grbl;
}

GrblCtrl::GrblCtrl()
{
}

void GrblCtrl::init()
{
#ifdef SERIAL2_SIMULATION
    strcpy(sim, "");
    idx = sim;
#endif
    strGrblIdx = 0;

    // initialise le port série vers grbl
    Serial2.begin(115200, SERIAL_8N1, SERIAL2_RXPIN, SERIAL2_TXPIN); // initialise le port série vers grbl
    Serial2.print(0X18);                                             // send a soft reset
    Serial2.println(" ");
    Serial2.print("$10=3");
    Serial2.println(" "); // $10=3 is used in order to get available space in GRBL buffer in GRBL status messages; il also means we are asking GRBL to sent always MPos.
    Serial2.flush();      // this is used to avoid sending to many jogging movements when using the nunchuk

    log_i("Grbl system ok.");
}

void GrblCtrl::error(const char *message)
{
}

void GrblCtrl::alarm(const char *message)
{
}

void GrblCtrl::status(const char *message)
{
}

int GrblCtrl::available()
{
#ifdef SERIAL2_SIMULATION
    if (*idx == 0)
    {
        int r = random(0, 65536);
        switch (r)
        {
        case 0:
            strcpy(sim, "<Idle|MPos:0.000,0.000,0.000|FS:0.0,0|WCO:0.000,0.000,0.000>\n");
            idx = sim;
            break;
        case 1:
            strcpy(sim, "<Jog|WPos:1329.142,0.000,0.000|Bf:32,254|FS:2000,0|Ov:100,100,100|A:FM>\n");
            idx = sim;
            break;
        case 2:
            strcpy(sim, "[Caution: Unlocked]\r\n");
            idx = sim;
            break;
        case 3:
            strcpy(sim, "error:Modal group violation\n\r");
            idx = sim;
            break;
        case 4:
            strcpy(sim, "<Idle,MPos:0.000,0.000,0.000,WPos:0.000,0.000,0.000>\n");
            idx = sim;
            break;
        default:
            return 0;
        }
    }
    return 1;
#else
    return Serial2.available();
#endif
}

int GrblCtrl::read()
{
#ifdef SERIAL2_SIMULATION
    int c = *idx;
    idx++;
    return c;
#else
    return Serial2.read();
#endif
}

// Capture serial data
void GrblCtrl::capture(void)
{
    int c;
    while (available())
    {
        c = read();
        if (c < 32)
        {
            // control char
            strGrblBuf[strGrblIdx++] = 0;
            flush();
        }
        else
        {
            if (strGrblIdx < STR_GRBL_BUF_MAX_SIZE - 1)
            {
                strGrblBuf[strGrblIdx++] = c;
            }
            else
            {
                // buffer overrun
                strGrblBuf[strGrblIdx++] = 0;
                flush();
            }
        }
    }
}

bool startWithNoCase(const char *str, const char *pattern)
{
    bool status = false;
    return status;
}

void GrblCtrl::flush(void)
{
    if (strlen(strGrblBuf) > 0)
    {
        // Remove case
        for (int i = 0; i < strGrblIdx; i++)
        {
            strGrblBufNoCase[i] = tolower(strGrblBuf[i]);
        }
        // Display data on TFT
        TFT_Screen::instance()->status(strGrblBuf);
        if (*strGrblBuf == '<')
        {
            decodeStatus(strGrblBuf, strGrblBufNoCase);
        }
        else
        {
            if (strncmp(strGrblBufNoCase, "error", 6))
            {
                decodeError(strGrblBuf, strGrblBufNoCase);
            }
            else
            {
                if (strncmp(strGrblBufNoCase, "alarm", 5))
                {
                    decodeAlarm(strGrblBuf, strGrblBufNoCase);
                }
                else
                {
                    if (strncmp(strGrblBufNoCase, "ok", 2))
                    {
                        decodeOk(strGrblBuf, strGrblBufNoCase);
                    }
                    else
                    {
                        if (*strGrblBuf == '[')
                        {
                            decodeFeedback(strGrblBuf, strGrblBufNoCase);
                        }
                    }
                }
            }
        }
    }
    // Reset index
    strGrblIdx = 0;
}

bool isLetter(char c)
{
    static char *letters = "abcdefghijklmnopqrstuvwxyz";
    for (int l = 0; l < 26; l++)
    {
        if (c == letters[l])
            return true;
    }
    return false;
}

const char *extract(const char *str, int sz)
{
    static char buffer[128];
    strncpy(buffer, str, sz);
    buffer[sz] = 0;
    return buffer;
}

void GrblCtrl::decodeStatus(const char *msg, const char *msgTolower)
{
    log_i("STATUS: '%s'", msg);
    char sep = ',';
    char subsep = ':';
    // begin at 1 ... ignore '<'
    int index = 1;
    int indexStatus = 1;
    for (; isLetter(msgTolower[index]); index++)
        ;
    log_i("STATUS/ext: '%s'", extract(&(msgTolower[1]), index - indexStatus));
    EvtCtrl::instance()->grblStatusEvent(extract(&(msgTolower[1]), index - indexStatus));
    sep = msgTolower[index];
}

void GrblCtrl::decodeError(const char *msg, const char *msgTolower)
{
}

void GrblCtrl::decodeAlarm(const char *msg, const char *msgTolower)
{
}

void GrblCtrl::decodeOk(const char *msg, const char *msgTolower)
{
}

void GrblCtrl::decodeFeedback(const char *msg, const char *msgTolower)
{
}

bool GrblCtrl::canWrite()
{
    return true;
}

void GrblCtrl::home()
{
    if (canWrite())
    {
        Serial2.println("$H");
    }
}

void GrblCtrl::unlock()
{
    if (canWrite())
    {
        Serial2.println("$X");
    }
}

void GrblCtrl::reset()
{
    if (canWrite())
    {
        Serial2.print((char)0x18);
    }
}

void GrblCtrl::pause()
{
    if (canWrite())
    {
        Serial2.print((char)'!');
    }
}

void GrblCtrl::resume()
{
    if (canWrite())
    {
        Serial2.print((char)'~');
    }
}

void GrblCtrl::move(GrblWay sens, float distance)
{
    if (canWrite())
    {
        log_i("Write %d %d", sens, distance);
        Serial2.println("");
        Serial2.print("$J=G91 G21 ");
        switch (sens)
        { // we convert the position of the button into the type of button
        case XP:
            Serial2.print("X");
            break;
        case XM:
            Serial2.print("X-");
            break;
        case YP:
            Serial2.print("Y");
            break;
        case YM:
            Serial2.print("Y-");
            break;
        case ZP:
            Serial2.print("Z");
            break;
        case ZM:
            Serial2.print("Z-");
            break;
        }
        Serial2.print(distance);
        Serial2.println(" F100");
    }
}

void GrblCtrl::setXYZ(GrblWay param)
{ // param contient le n° de la commande
    switch (param)
    {
    case SETX:
        Serial2.println("G10 L20 P1 X0");
        break;
    case SETY:
        Serial2.println("G10 L20 P1 Y0");
        break;
    case SETZ:
        Serial2.println("G10 L20 P1 Z0");
        break;
    case SETXYZ:
        Serial2.println("G10 L20 P1 X0 Y0 Z0");
        break;
    }
}

// Submit event localy created by widget
void GrblCtrl::submit(Event *event)
{
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_CTRL_JOYSTICK + 1)
    {
        this->move(XM, 1.0);
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_CTRL_JOYSTICK + 2)
    {
        this->move(XP, 1.0);
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_CTRL_JOYSTICK + 3)
    {
        this->move(YM, 1.0);
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_CTRL_JOYSTICK + 4)
    {
        this->move(YP, 1.0);
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_CTRL_JOYSTICK + 5)
    {
        this->move(ZM, 1.0);
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_CTRL_JOYSTICK + 6)
    {
        this->move(ZP, 1.0);
    }
}
