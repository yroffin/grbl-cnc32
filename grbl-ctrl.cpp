#include "grbl-ctrl.hpp"
#include "storage-ctrl.hpp"
#include "stdio.h"

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
    strcpy(sim, "");
    idx = sim;
    this->simulation = false;

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
    if (this->simulation)
    {
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
    }
    else
    {
        return Serial2.available();
    }
}

int GrblCtrl::read()
{
    if (simulation)
    {
        int c = *idx;
        idx++;
        return c;
    }
    else
    {
        return Serial2.read();
    }
}

// Capture serial data
void GrblCtrl::capture(void)
{
    int c;
    while (available())
    {
        c = read();
        this->byteRead++;
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
    // Update statistics
    TFT_Screen::instance()->statistic->setGrblIo("b: %06d r: %06d w: %06d", this->byteRead, this->txRead, this->txWrite);
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
        this->txRead++;
        // Remove case
        for (int i = 0; i < strGrblIdx; i++)
        {
            strGrblBufNoCase[i] = tolower(strGrblBuf[i]);
        }
        uint8_t type = 0;
        if (*strGrblBuf == '<')
        {
            type = 1;
        }
        else
        {
            if (strncmp(strGrblBufNoCase, "error", 6) == 0)
            {
                type = 2;
            }
            else
            {
                if (strncmp(strGrblBufNoCase, "alarm", 5) == 0)
                {
                    type = 3;
                }
                else
                {
                    if (strncmp(strGrblBufNoCase, "ok", 2) == 0)
                    {
                        type = 4;
                    }
                    else
                    {
                        if (*strGrblBuf == '[')
                        {
                            type = 5;
                        }
                    }
                }
            }
        }
        switch (type)
        {
        case 1:
            decodeStatus(strGrblBuf, strGrblBufNoCase);
            break;
        case 2:
            decodeError(strGrblBuf, strGrblBufNoCase);
            break;
        case 3:
            decodeAlarm(strGrblBuf, strGrblBufNoCase);
            break;
        case 4:
            decodeOk(strGrblBuf, strGrblBufNoCase);
            break;
        case 5:
            decodeFeedback(strGrblBuf, strGrblBufNoCase);
            break;
        default:
            // Display data on TFT
            TFT_Screen::instance()->status("[GRBL] %04x %s", this->txRead & 0xFFFF, strGrblBuf);
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
    EvtCtrl::instance()->grblStatusEvent(WIDGET_ID_GRBL, extract(&(msgTolower[1]), index - indexStatus));
    sep = msgTolower[index];
}

void GrblCtrl::decodeError(const char *msg, const char *msgTolower)
{
    TFT_Screen::instance()->status("[ERROR] %s", msg);
}

void GrblCtrl::decodeAlarm(const char *msg, const char *msgTolower)
{
    TFT_Screen::instance()->status("[ALARM] %s", msg);
}

void GrblCtrl::decodeOk(const char *msg, const char *msgTolower)
{
    TFT_Screen::instance()->status("[OK] %s", msg);
    this->busy = false;
}

void GrblCtrl::decodeFeedback(const char *msg, const char *msgTolower)
{
    TFT_Screen::instance()->status("[FEEDBACK] %s", msg);
}

void GrblCtrl::write(const char *grbl, ...)
{
    this->txWrite++;
    va_list args;
    va_start(args, grbl);
    char buffer[STR_GRBL_BUF_MAX_WRITE_SIZE];
    vsprintf(buffer, grbl, args);
    if (this->simulation)
    {
        TFT_Screen::instance()->status("[SIMUL]  %06d %s (force)", this->txWrite, buffer);
    }
    else
    {
        TFT_Screen::instance()->status("[WRITE] %06d %s (force)", this->txWrite, buffer);
        Serial2.print(buffer);
    }
    va_end(args);
}

bool GrblCtrl::tryWrite(const char *grbl, ...)
{
    va_list args;
    va_start(args, grbl);
    char buffer[STR_GRBL_BUF_MAX_WRITE_SIZE];
    vsprintf(buffer, grbl, args);
    if (this->busy)
    {
        TFT_Screen::instance()->status("[BUSY] %s", buffer);
        return false;
    }
    else
    {
        this->txWrite++;
        if (this->simulation)
        {
            TFT_Screen::instance()->status("[SIMUL] %06d %s", this->txWrite, buffer);
            this->busy = true;
        }
        else
        {
            TFT_Screen::instance()->status("[WRITE] %06d %s", this->txWrite, buffer);
            Serial2.print(buffer);
            this->busy = true;
        }
        va_end(args);
        return true;
    }
}

void GrblCtrl::print(const char *filename)
{
    StorageCtrl::instance()->open(filename);
    this->isPrinting = true;
}

void GrblCtrl::spool()
{
    if (this->isPrinting)
    {
        boolean avail = StorageCtrl::instance()->readline(this->printBuffer, STR_GRBL_BUF_MAX_SIZE);
        if (avail)
        {
            TFT_Screen::instance()->printing("> %s", this->printBuffer);
        }
        else
        {
            this->isPrinting = false;
            StorageCtrl::instance()->close();
        }
    }
}

boolean GrblCtrl::home()
{
    return this->tryWrite("$H\n");
}

boolean GrblCtrl::unlock()
{
    this->write("$X\n");
    return true;
}

boolean GrblCtrl::reset()
{
    this->write("%c\n", 0x18);
    return true;
}

boolean GrblCtrl::pause()
{
    return this->tryWrite("!\n");
}

boolean GrblCtrl::resume()
{
    return this->tryWrite("~\n");
}

boolean GrblCtrl::status()
{
    this->write("?\n");
    return true;
}

boolean GrblCtrl::move(GrblWay sens, float distance)
{
    switch (sens)
    { // we convert the position of the button into the type of button
    case XP:
        return this->tryWrite("$J=G91 G21 X%f F100\n", distance);
        break;
    case XM:
        return this->tryWrite("$J=G91 G21 X-%f F100\n", distance);
        break;
    case YP:
        return this->tryWrite("$J=G91 G21 Y%f F100\n", distance);
        break;
    case YM:
        return this->tryWrite("$J=G91 G21 Y-%f F100\n", distance);
        break;
    case ZP:
        return this->tryWrite("$J=G91 G21 Z%f F100\n", distance);
        break;
    case ZM:
        return this->tryWrite("$J=G91 G21 Z-%f F100\n", distance);
        break;
    }
}

boolean GrblCtrl::setXYZ(GrblWay param)
{ // param contient le n° de la commande
    switch (param)
    {
    case SETX:
        return this->tryWrite("G10 L20 P1 X0\n");
        break;
    case SETY:
        return this->tryWrite("G10 L20 P1 Y0\n");
        break;
    case SETZ:
        return this->tryWrite("G10 L20 P1 Z0\n");
        break;
    case SETXYZ:
        return this->tryWrite("G10 L20 P1 X0 Y0 Z0\n");
        break;
    }
}

// Notify
void GrblCtrl::notify(const Event *event)
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
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_CTRL_HOME)
    {
        this->home();
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_CTRL_UNLOCK)
    {
        this->unlock();
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_CTRL_RESET)
    {
        this->reset();
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_CTRL_RESUME)
    {
        this->resume();
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_CTRL_PAUSE)
    {
        this->pause();
    }
    if (event->type == buttonDown && event->sender == WIDGET_ID_LAYER_CTRL_STATUS)
    {
        this->status();
    }
}
