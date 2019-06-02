#include "ui.hpp"
#include "i18n-ctrl.hpp"
#include "grbl-ctrl.hpp"
#include "storage-ctrl.hpp"
#include "utils.hpp"

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
    Utils::strcpy(sim, "", MAXSIZE_OF_SIM);
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

    log_i("%s", I18nCtrl::instance()->translate(I18N_STD, I18N_OK, "GRBL"));
}

// Scan new available bytes
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
                Utils::strcpy(sim, "<Idle|MPos:1.000,0.200,0.030|FS:0.0,0|WCO:8.000,0.700,0.006>\n", MAXSIZE_OF_SIM);
                idx = sim;
                break;
            case 1:
                Utils::strcpy(sim, "<Jog|WPos:1329.142,0.580,1.000|Bf:32,254|FS:2000,0|Ov:101,102,100|A:FM>\n", MAXSIZE_OF_SIM);
                idx = sim;
                break;
            case 2:
                Utils::strcpy(sim, "[Caution: Unlocked]\r\n", MAXSIZE_OF_SIM);
                idx = sim;
                break;
            case 3:
                Utils::strcpy(sim, "error:Modal group violation\n\r", MAXSIZE_OF_SIM);
                idx = sim;
                break;
            case 4:
                Utils::strcpy(sim, "<Idle,MPos:0.000,0.000,0.000,WPos:0.000,0.000,0.000>\n", MAXSIZE_OF_SIM);
                idx = sim;
                break;
            case 5:
                Utils::strcpy(sim, "error:20\n\r", MAXSIZE_OF_SIM);
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

// Read one byte
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

// write
void GrblCtrl::write(boolean flush, const char *value)
{
    // write data
    Serial2.print(value);
    // wait for send all data
    if (flush)
        Serial2.flush();
    TFT_Screen::instance()->menu->status->notifyWrite((uint16_t)strlen(value));
}

void GrblCtrl::setBusy(boolean _busyState)
{
    this->busy = _busyState;
    TFT_Screen::instance()->menu->status->notifyBusy(this->busy);
}

boolean GrblCtrl::isBusy()
{
    return this->busy;
}

// Capture serial data
void GrblCtrl::capture(void)
{
    if (simulation)
    {
        this->setBusy(false);
    }
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

// string function
bool startWithNoCase(const char *str, const char *pattern)
{
    bool status = false;
    return status;
}

// Flush
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
            if (strncmp(strGrblBufNoCase, "error", 5) == 0)
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
        TFT_Screen::instance()->grblOutputConsole(strGrblBuf);
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
            TFT_Screen::instance()->outputConsole("[GRBL] %04x %s", this->txRead & 0xFFFF, strGrblBuf);
        }
    }
    // Reset index
    strGrblIdx = 0;
}

bool isLetter(char c)
{
    static char *matcher = "abcdefghijklmnopqrstuvwxyz";
    for (int l = 0; l < 26; l++)
    {
        if (c == matcher[l])
            return true;
    }
    return false;
}

bool isNumber(char c)
{
    static char *matcher = "0123456789.";
    for (int l = 0; l < 11; l++)
    {
        if (c == matcher[l])
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

boolean match(const char *pattern, const char *value)
{
    return strncmp(pattern, value, strlen(pattern)) == 0;
}

// Scan position
boolean scanPos(const char *pattern, EventType e, const char *value)
{
    if (match(pattern, value))
    {
        float fvalue1, fvalue2, fvalue3;
        sscanf(&(value[strlen(pattern)]), "%f,%f,%f", &fvalue1, &fvalue2, &fvalue3);
        EvtCtrl::instance()->sendWithVector(0, e, fvalue1, fvalue2, fvalue3);
    }
}

// Scan error
boolean scanError(const char *pattern, EventType e, const char *value)
{
    if (match(pattern, value))
    {
        int code;
        if (isNumber(value[strlen(pattern)]))
        {
            sscanf(&(value[strlen(pattern)]), "%d", &code);
            const char *msg = I18nCtrl::instance()->translate(I18N_GRBL, code);
            EvtCtrl::instance()
                ->sendWithString(0, e, msg);
        }
        else
        {
            EvtCtrl::instance()->sendWithString(0, e, &(value[strlen(pattern)]));
        }
    }
}

// Decode status
void GrblCtrl::decodeStatus(const char *msg, const char *msgTolower)
{
    char sep = ',';
    char subsep = ':';
    // begin at 1 ... ignore '<'
    int index = 1;
    int indexStatus = 1;
    // find separator and status
    for (; isLetter(msgTolower[index]); index++)
        ;
    this->grblState = GRBL_UNKNOWN;
    if (strcmp(GRBL_STATUS_IDLE, extract(&(msgTolower[1]), index - indexStatus)) == 0)
    {
        this->grblState = GRBL_IDLE;
    }
    if (strcmp(GRBL_STATUS_RUN, extract(&(msgTolower[1]), index - indexStatus)) == 0)
    {
        this->grblState = GRBL_RUN;
    }
    EvtCtrl::instance()->sendInt(WIDGET_ID_GRBL, EVENT_GRBL_STATUS, this->grblState);
    char separator = msgTolower[index];
    // now find block
    const char *block = &(msgTolower[index + 1]);
    int blkl = strlen(block);
    for (int b = 0; b < blkl; b++)
    {
        scanPos("mpos:", EVENT_MPOS, &(block[b]));
        scanPos("wpos:", EVENT_WPOS, &(block[b]));
    }
    sep = msgTolower[index];
}

void GrblCtrl::decodeError(const char *msg, const char *msgTolower)
{
    scanError("error:", EVENT_ERROR, msgTolower);
}

void GrblCtrl::decodeAlarm(const char *msg, const char *msgTolower)
{
    TFT_Screen::instance()->outputConsole("[ALARM] %s", msg);
}

void GrblCtrl::decodeOk(const char *msg, const char *msgTolower)
{
    // ok so don't busy anymore
    this->setBusy(false);
}

void GrblCtrl::decodeFeedback(const char *msg, const char *msgTolower)
{
    TFT_Screen::instance()->outputConsole("[FEEDBACK] %s", msg);
}

// force write
void GrblCtrl::forceWrite(boolean flush, const char *grbl, ...)
{
    this->txWrite++;
    va_list args;
    va_start(args, grbl);
    // use utils buffer to protect memory
    vsprintf(Utils::vsprintfBuffer(), grbl, args);
    va_end(args);
    Utils::strcpy(this->writeBuffer, Utils::vsprintfBuffer(), STR_GRBL_BUF_MAX_WRITE_SIZE);
    TFT_Screen::instance()->grblInputConsole(this->writeBuffer);
    this->write(flush, this->writeBuffer);
}

// try to write (only when not busy)
bool GrblCtrl::tryWrite(boolean flush, const char *grbl, ...)
{
    va_list args;
    va_start(args, grbl);
    // use utils buffer to protect memory
    vsprintf(Utils::vsprintfBuffer(), grbl, args);
    Utils::strcpy(this->writeBuffer, Utils::vsprintfBuffer(), STR_GRBL_BUF_MAX_WRITE_SIZE);
    va_end(args);
    // ignore comment ( / ;
    switch (*this->writeBuffer)
    {
    case '(':
    case '/':
    case ';':
        return true;
    }
    // check busy state
    if (this->isBusy())
    {
        return false;
    }
    else
    {
        this->txWrite++;
        this->setBusy(true);
        this->write(flush, this->writeBuffer);
        TFT_Screen::instance()->grblInputConsole(this->writeBuffer);
        return true;
    }
}

// start printing
void GrblCtrl::print(const char *filename)
{
    TFT_Screen::instance()->outputConsole("printing: %s", filename);
    this->toPrintLines = StorageCtrl::instance()->open(filename);
    this->printedLines = 0;
    this->isPrinting = true;
    this->grblPrintStatus = empty;
}

// printer spooler
void GrblCtrl::spool()
{
    if (this->isPrinting && !this->isPaused)
    {
        switch (this->grblPrintStatus)
        {
        case empty:
            // send status only when ready, in order to not flood grbl controller
            if (millis() - this->lastStatus > 2000)
            {
                this->lastStatus = millis();
                this->status();
                this->grblPrintStatus = waitForStatus;
            }
            else
            {
                // send a new command to grbl
                if (StorageCtrl::instance()->readline(this->printBuffer, STR_GRBL_BUF_MAX_SIZE))
                {
                    this->grblPrintStatus = full;
                    this->printedLines++;
                    if (this->printedLines % 4)
                    {
                        TFT_Screen::instance()->notifyPrintStatus(true, this->printedLines, this->toPrintLines);
                    }
                }
                else
                {
                    // no more data so stop printing
                    TFT_Screen::instance()->outputConsole("stop printing");
                    this->isPrinting = false;
                    this->grblPrintStatus = empty;
                    StorageCtrl::instance()->close();
                }
            }
            break;
        case full:
            if (!this->isBusy())
            {
                // command is waiting for sending
                if (this->tryWrite(true, this->printBuffer))
                {
                    this->grblPrintStatus = empty;
                }
            }
            break;
        case waitForStatus:
            // status needed to know what we are doing
            if (!this->isBusy())
            {
                this->grblPrintStatus = empty;
            }
            break;
        }
    }
    // while non printing compute status
    if (!this->isPrinting)
    {
        // send status only when ready, in order to not flood grbl controller
        if (millis() - this->lastStatus > 1000)
        {
            this->lastStatus = millis();
            this->status();
        }
    }
}

// home command
boolean GrblCtrl::home()
{
    return this->tryWrite(true, "$H\n");
}

// unlock command
boolean GrblCtrl::unlock()
{
    this->forceWrite(true, "$X\n");
    return true;
}

// reset command
boolean GrblCtrl::reset()
{
    // stop printing
    this->isPrinting = false;
    // and reset
    this->forceWrite(true, "%c\n", 0x18);
    return true;
}

// pause command
boolean GrblCtrl::pause()
{
    this->forceWrite(true, "!\n");
    this->isPaused = true;
    return true;
}

// resume command
boolean GrblCtrl::resume()
{
    this->forceWrite(true, "~\n");
    this->isPaused = false;
    return true;
}

// status
boolean GrblCtrl::status()
{
    this->forceWrite(true, "?\n");
    return true;
}

// move command
boolean GrblCtrl::move(EventGrbl sens, float distance)
{
    switch (sens)
    { // we convert the position of the button into the type of button
    case XP:
        return this->tryWrite(true, "$J=G91 G21 X%.2f F100\n", distance);
        break;
    case XM:
        return this->tryWrite(true, "$J=G91 G21 X-%.2f F100\n", distance);
        break;
    case YP:
        return this->tryWrite(true, "$J=G91 G21 Y%.2f F100\n", distance);
        break;
    case YM:
        return this->tryWrite(true, "$J=G91 G21 Y-%.2f F100\n", distance);
        break;
    case ZP:
        return this->tryWrite(true, "$J=G91 G21 Z%.2f F100\n", distance);
        break;
    case ZM:
        return this->tryWrite(true, "$J=G91 G21 Z-%.2f F100\n", distance);
        break;
    }
}

// move command
// speed in pourcent of max speed
boolean GrblCtrl::move(float x, float y, float z, int speed)
{
    return this->tryWrite(true, "$J=G91 G21 X%.2f Y%.2f Z%.2f F%d\n", x, y, z, speed);
}

// set command
boolean GrblCtrl::setXYZ(EventGrbl param)
{
    switch (param)
    {
    case SETX:
        return this->tryWrite(true, "G10 L20 P1 X0\n");
        break;
    case SETY:
        return this->tryWrite(true, "G10 L20 P1 Y0\n");
        break;
    case SETZ:
        return this->tryWrite(true, "G10 L20 P1 Z0\n");
        break;
    case SETXYZ:
        return this->tryWrite(true, "G10 L20 P1 X0 Y0 Z0\n");
        break;
    }
}

// compute next step
GrblStep nextStep(GrblStep current)
{
    switch (current)
    {
    case M1:
        return M10;
        break;
    case M10:
        return M100;
        break;
    case M100:
        return M1;
        break;
    }
}

// notify handler
void GrblCtrl::notify(const Event *event)
{
    if (event->type == EVENT_ERROR)
    {
        TFT_Screen::instance()->outputConsole("[ERROR] %s", event->message);
    }
    if (event->type == JOG_STOP)
    {
        this->tryWrite(true, "%c\n", 0x85);
    }
    if (event->type == JOG_MOVEXY)
    {
        this->move(
            event->touch.x / 10.,
            (float)event->touch.y / 10.,
            0.,
            2000);
    }
    if (event->type == JOG_MOVEZ)
    {
        this->move(
            0.,
            0.,
            event->touch.y / 10.,
            400);
    }
    if (event->type == EVENT_NEXT_STEP)
    {
        this->step = nextStep(this->step);
        switch (this->step)
        {
        case M1:
            this->xyzJogPas = 1;
            break;
        case M10:
            this->xyzJogPas = 5;
            break;
        case M100:
            this->xyzJogPas = 50;
            break;
        }
        EvtCtrl::instance()->sendInt(0, EVENT_NEW_STEP, this->xyzJogPas);
    }
    if (event->type == BUTTON_DOWN && event->sender == WIDGET_ID_LAYER_CTRL_HOME)
    {
        this->home();
    }
    if (event->type == BUTTON_DOWN && event->sender == WIDGET_ID_LAYER_ADM_UNLOCK)
    {
        this->unlock();
    }
    if (event->type == BUTTON_DOWN && event->sender == WIDGET_ID_LAYER_ADM_RESET)
    {
        this->reset();
    }
    if (event->type == BUTTON_DOWN && event->sender == WIDGET_ID_LAYER_ADM_RESUME)
    {
        this->resume();
    }
    if (event->type == BUTTON_DOWN && event->sender == WIDGET_ID_LAYER_ADM_PAUSE)
    {
        this->pause();
    }
    if (event->type == BUTTON_DOWN && event->sender == WIDGET_ID_LAYER_ADM_STATUS)
    {
        this->status();
    }
    if (event->type == EVENT_SETX)
    {
        this->setXYZ(SETX);
    }
    if (event->type == EVENT_SETY)
    {
        this->setXYZ(SETY);
    }
    if (event->type == EVENT_SETZ)
    {
        this->setXYZ(SETZ);
    }
    if (event->type == EVENT_SETXYZ)
    {
        this->setXYZ(SETXYZ);
    }
    // cancel current jog
    if (this->lastJog != 0 && millis() - this->lastJog > 1000)
    {
        this->lastJog = 0;
        EvtCtrl::instance()->send(
            WIDGET_ID_DEFAULT,
            JOG_STOP);
    }
}

// jog mode xy
void GrblCtrl::jogMoveXY(int16_t x, int16_t y)
{
    EvtCtrl::instance()->sendTouch(
        WIDGET_ID_DEFAULT,
        JOG_MOVEXY,
        x,
        y);
    this->lastJog = millis();
}

// jog mode z
void GrblCtrl::jogMoveZ(int16_t z)
{
    EvtCtrl::instance()->sendTouch(
        WIDGET_ID_DEFAULT,
        JOG_MOVEZ,
        0,
        z);
    this->lastJog = millis();
}