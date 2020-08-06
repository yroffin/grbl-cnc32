#include "nunchuk-ctrl.hpp"
#include "grbl-ctrl.hpp"
#include "evt-ctrl.hpp"
#include "json-config.hpp"

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

void NunchukCtrl::setup()
{
    JsonConfigCtrl *jsonConfig = JsonConfigCtrl::instance();
    this->xcenter = jsonConfig->getAsInt("nunchuk", "calibrate", "x", 134);
    this->ycenter = jsonConfig->getAsInt("nunchuk", "calibrate", "y", 134);
    this->uTime = jsonConfig->getAsInt("fingerprint", "uTime", 0);

    this->xyzL1Pas = jsonConfig->getAsFloat("nunchuk", "speed", "l1", 0.1);
    this->xyzL2Pas = jsonConfig->getAsFloat("nunchuk", "speed", "l2", 5.0);
}

void NunchukCtrl::loop()
{
    JsonConfigCtrl *jsonConfig = JsonConfigCtrl::instance();
    if (jsonConfig->getAsInt("fingerprint", "uTime", 0) != this->uTime)
    {
        this->xcenter = jsonConfig->getAsInt("nunchuk", "calibrate", "x", 134);
        this->ycenter = jsonConfig->getAsInt("nunchuk", "calibrate", "y", 134);
        this->uTime = jsonConfig->getAsInt("fingerprint", "uTime", 0);
    }
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
        if (!read())
        {
            break;
        }
        this->wait = millis();
        this->state = NUNCHUK_OK;

        {
            // save current state
            boolean current_z = this->z;
            boolean current_c = this->c;
            int16_t current_xplane = this->xplane;
            int16_t current_yplane = this->yplane;

            // new state
            this->z = Z();
            this->c = C();

            // raw value
            if (this->xplane != (nunchuk_data[0] - this->xcenter) || this->yplane != (nunchuk_data[1] - this->ycenter))
            {
                this->xplane = nunchuk_data[0] - this->xcenter;
                this->yplane = nunchuk_data[1] - this->ycenter;
            }

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
            if (c && !z)
            {
                if (abs(xplane) >= 10 || abs(yplane) >= 10)
                {
                    float xlader = abs(xplane) < 10 ? 0. : abs(xplane) < 60 ? this->xyzL1Pas : this->xyzL2Pas;
                    float ylader = abs(yplane) < 10 ? 0. : abs(yplane) < 60 ? this->xyzL1Pas : this->xyzL2Pas;
                    GrblCtrl::instance()->jogMoveXY(xplane < 0 ? -xlader : xlader, yplane < 0 ? -ylader : ylader);
                }
            }

            if (!c && z)
            {
                if (abs(yplane) >= 10)
                {
                    float ylader = abs(yplane) < 10 ? 0. : abs(yplane) < 60 ? this->xyzL1Pas : this->xyzL2Pas;
                    GrblCtrl::instance()->jogMoveZ(yplane < 0 ? -ylader : ylader);
                }
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
    // error when all received bytes are 00 or FF.
    if (error == 0b00111111)
    {
        disableEncription(); // try to reactivate nunchunck in case of error.
    }
    I2C_START(NUNCHUK_ADDRESS);
    I2C_WRITE(0x00);
    I2C_STOP();
    return !(error == 0b00111111); // return true when there is no error
}

boolean NunchukCtrl::Z()
{ // Checks the current state of button Z
    return (~nunchuk_data[5]) & 1 == 1;
}

boolean NunchukCtrl::C()
{ // Checks the current state of button C
    return (~nunchuk_data[5] >> 1) & 1 == 1;
}
