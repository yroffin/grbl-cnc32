#include "grbl-ctrl.hpp"

GrblCtrl::GrblCtrl(TFT_Screen &_tft, EvtCtrl &_evtCtrl) : tft(_tft), evtCtrl(_evtCtrl)
{
    log_i("Grbl controller ok.");
}

void GrblCtrl::init()
{
    strcpy(sim, "");
    idx = sim;
    strGrblIdx = 0;
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
    if (SERIAL2_SIMULATION)
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
    if (SERIAL2_SIMULATION)
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
        tft.status(strGrblBuf);
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
    evtCtrl.grblStatusEvent(extract(&(msgTolower[1]), index - indexStatus));
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
