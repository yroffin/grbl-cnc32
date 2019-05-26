#include "nunchuk-ctrl.hpp"
#include "evt-ctrl.hpp"

// NunchukCtrl controller
NunchukCtrl *__instance_NunchukCtrl = 0;

// singleton
NunchukCtrl *NunchukCtrl::instance()
{
    if (__instance_NunchukCtrl == 0)
    {
        __instance_NunchukCtrl = new NunchukCtrl();
    }
    return __instance_NunchukCtrl;
}

NunchukCtrl::NunchukCtrl()
{
}

void NunchukCtrl::init()
{
}

int16_t NunchukCtrl::lader(int16_t value, int16_t l0, int16_t l1, int16_t l10, int16_t l100)
{
    if (value > 0)
    {
        if (value < l0)
        {
            return 0;
        }
        if (value < l1)
        {
            return 1;
        }
        if (value < l10)
        {
            return 10;
        }
        if (value < l100)
        {
            return 100;
        }
        return 100;
    }
    else
    {
        if (value < -l100)
        {
            return -100;
        }
        if (value < -l10)
        {
            return -10;
        }
        if (value < -l1)
        {
            return -1;
        }
        if (value < -l0)
        {
            return 0;
        }
        return 0;
    }
    return 0;
}

void NunchukCtrl::capture()
{
    switch (this->state)
    {
    case NUNCHUK_STATE_ZERO:
        Wire.begin();
        // Change TWI speed for nunchuk, which uses Fast-TWI (400kHz)
        // Normally this will be set in twi_init(), but this hack works without modifying the original source
        Wire.setClock(400000);
        this->wait = millis();
        this->state = NUNCHUK_STATE_WAIT;
        break;
    case NUNCHUK_STATE_WAIT:
        if (millis() - this->wait > 500)
        {
            this->wait = millis();
            this->state = NUNCHUK_STATE_INIT;
        }
        break;
    case NUNCHUK_STATE_INIT:
        Wire.beginTransmission(NUNCHUK_ADDRESS);
        if (Wire.endTransmission())
        {
            this->state = NUNCHUK_KO;
        }
        else
        {
            this->wait = millis();
            this->state = NUNCHUK_STATE_CAN_READ;
        }
        break;
    case NUNCHUK_STATE_CAN_READ:
        disableEncription();
        read();
        delay(20);
        this->wait = millis();
        this->state = NUNCHUK_STATE_START_READ;
        break;
    case NUNCHUK_STATE_START_READ:
        read();
        this->wait = millis();
        this->state = NUNCHUK_OK;

        {
            // save current state
            boolean current_z = this->z;
            boolean current_c = this->c;
            int16_t current_xplane = this->xplane;
            int16_t current_yplane = this->yplane;
            int16_t current_xlader = this->xlader;
            int16_t current_ylader = this->ylader;
            int16_t current_xcenter = this->xcenter;
            int16_t current_ycenter = this->ycenter;

            // new state
            this->z = Z();
            this->c = C();

            // Calibrate
            if (this->c && this->z)
            {
                this->xcenter = this->nunchuk_data[0];
                this->ycenter = this->nunchuk_data[1];
            }

            // raw value
            if (this->xplane != (nunchuk_data[0] - this->xcenter) || this->yplane != (nunchuk_data[1] - this->ycenter))
            {
                this->xplane = nunchuk_data[0] - this->xcenter;
                this->yplane = nunchuk_data[1] - this->ycenter;
            }

            // lader value
            this->xlader = this->lader(xplane, 5, 25, 70, 90);
            this->ylader = this->lader(yplane, 5, 25, 70, 90);

            // check change on Z and C
            if (current_z != z)
            {
                EvtCtrl::instance()->sendInt(WIDGET_ID_DEFAULT, NUNCHUK_Z, z);
            }

            if (current_c != c)
            {
                EvtCtrl::instance()->sendInt(WIDGET_ID_DEFAULT, NUNCHUK_C, c);
            }

            // check change on xplane and yplane
            if (current_xplane != xplane || current_yplane != yplane)
            {
                EvtCtrl::instance()->sendTouch(WIDGET_ID_DEFAULT, NUNCHUK_DATA, xplane, yplane);
            }

            // lader value
            boolean click = (current_z != z || current_c != c);
            if (c && z && click)
            {
                EvtCtrl::instance()->sendTouch(WIDGET_ID_DEFAULT, NUNCHUK_CALIBRATE, xlader, ylader);
            }

            if (c && !z)
            {
                EvtCtrl::instance()->sendTouch(WIDGET_ID_DEFAULT, NUNCHUK_LADER_MOVEXY, xlader, ylader);
            }

            if (!c && z)
            {
                EvtCtrl::instance()->sendTouch(WIDGET_ID_DEFAULT, NUNCHUK_LADER_MOVEZ, xlader, ylader);
            }
        }
        break;

    case NUNCHUK_OK:
        if (millis() - this->wait > 20)
        {
            this->wait = millis();
            this->state = NUNCHUK_STATE_START_READ;
        }
        break;

    case NUNCHUK_KO:
        log_e("nunchuk %d", this->state);
        break;
    }
}

void NunchukCtrl::disableEncription()
{
    I2C_START(NUNCHUK_ADDRESS); // disable encription with next 2 commands
    I2C_WRITE(0xF0);
    I2C_WRITE(0x55);
    I2C_STOP();
    I2C_START(NUNCHUK_ADDRESS);
    I2C_WRITE(0xFB);
    I2C_WRITE(0x00);
    I2C_STOP();
}

boolean NunchukCtrl::read()
{
    uint8_t i;
    uint8_t error = 0;
    Wire.requestFrom(NUNCHUK_ADDRESS, 6);
    for (i = 0; i < 6 && Wire.available(); i++)
    {
        nunchuk_data[i] = I2C_READ();
        if (nunchuk_data[i] == 0 || nunchuk_data[i] == 0xFF)
        {
            error |= (1 << i);
        }
    }
    error = (error == 0b00111111); // error when all received bytes are 00 or FF.
    if (error)
    {
        log_e("nunchuk error %d", error);
        disableEncription(); // try to reactivate nunchunck in case of error.
    }
    I2C_START(NUNCHUK_ADDRESS);
    I2C_WRITE(0x00);
    I2C_STOP();
    return !error; // return true when there is no error
}

boolean NunchukCtrl::Z()
{ // Checks the current state of button Z
    return (~nunchuk_data[5]) & 1 == 1;
}

boolean NunchukCtrl::C()
{ // Checks the current state of button C
    return (~nunchuk_data[5] >> 1) & 1 == 1;
}
