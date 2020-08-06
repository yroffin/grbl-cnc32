#ifndef _NUNCHUK_CONTROLLER_
#define _NUNCHUK_CONTROLLER_

#include "Arduino.h"
#include "Wire.h"

enum NUNCHUK_STATE
{
  NUNCHUK_STATE_ZERO,
  NUNCHUK_STATE_WAIT,
  NUNCHUK_STATE_INIT,
  NUNCHUK_KO,
  NUNCHUK_STATE_CAN_READ,
  NUNCHUK_STATE_START_READ,
  NUNCHUK_OK
};

// The Nunchuk I2C address
#define NUNCHUK_ADDRESS 0x52

#if ARDUINO >= 100
#define I2C_READ() Wire.read()
#define I2C_WRITE(x) Wire.write(x)
#else
#define I2C_READ() Wire.receive()
#define I2C_WRITE(x) Wire.send(x)
#endif

#define I2C_START(x) Wire.beginTransmission(x)
#define I2C_STOP() Wire.endTransmission(true)

class NunchukCtrl
{
public:
  NunchukCtrl();
  static NunchukCtrl *instance();

  void setup();
  void loop();

  void disableEncription();
  boolean read();

  boolean Z();
  boolean C();

private:
  NUNCHUK_STATE state = NUNCHUK_STATE_ZERO;
  long wait = 0;
  int16_t nunchuk_data[6];
  boolean z = false;
  boolean c = false;
  int16_t xplane = 0;
  int16_t yplane = 0;
  int16_t xcenter = 0;
  int16_t ycenter = 0;
  float xyzL1Pas = 0.1;
  float xyzL2Pas = 5.0;
  int uTime = 0;
};

#endif
