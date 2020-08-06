#ifndef _GRBL_CONTROLLER_
#define _GRBL_CONTROLLER_

#include "config.h"
#include "ui.hpp"
#include "evt-ctrl.hpp"

// GRBL status are : Idle, Run, Hold, Jog, Alarm, Door, Check, Home, Sleep
// a message should look like (note : GRBL sent or WPOS or MPos depending on grbl parameter : to get WPos, we have to set "$10=0"
//  <Jog|WPos:1329.142,0.000,0.000|Bf:32,254|FS:2000,0|Ov:100,100,100|A:FM>
//  <Idle|WPos:0.000,0.000,0.000|FS:0.0,0> or e.g. <Idle|MPos:0.000,0.000,0.000|FS:0.0,0|WCO:0.000,0.000,0.000>
//CLOSED
//   START
//        WPOS_HEADER
//            WPOS_DATA
//                               HEADER
//                                 F_DATA
//                                   F_DATA
//                                                                                      WCO_DATA
// note : status can also be with a ":" like Hold:1
// outside <...>, we can get "Ok" and "error:12" followed by an CR and/or LF
// we can also get messages: They are enclosed between [....] ; they are currently discarded.
// used to decode the status and position sent by GRBL

enum GrblStatus
{
  GRBL_UNKNOWN,
  GRBL_IDLE,
  GRBL_RUN,
  GRBL_HOLD,
  GRBL_JOG,
  GRBL_ALARM,
  GRBL_DOOR,
  GRBL_CHECK,
  GRBL_HOME,
  GRBL_SLEEP
};

#define MAXSIZE_OF_SIM 512
#define GRBL_STATUS_IDLE "idle"
#define GRBL_STATUS_RUN "run"
#define GRBL_STATUS_HOLD "hold"
#define GRBL_STATUS_JOG "jog"
#define GRBL_STATUS_ALARM "alarm"
#define GRBL_STATUS_DOOR "door"
#define GRBL_STATUS_CHECK "check"
#define GRBL_STATUS_HOME "home"
#define GRBL_STATUS_SLEEP "sleep"

#define GRBL_STATE_G20 "g20"
#define GRBL_STATE_G21 "g21"
#define GRBL_STATE_G90 "g90"
#define GRBL_STATE_G91 "g91"

#define STR_GRBL_BUF_MAX_SIZE 256
#define STR_GRBL_BUF_MAX_WRITE_SIZE 256

enum GrblPrintStatus
{
  empty,
  full,
  waitForStatus
};

enum GrblStep
{
  M1,
  M10,
  M100
};

class GrblCtrl
{
public:
  GrblCtrl();

  void setup();
  void loop();

  void print(const char *filename);
  void remove(const char *filename);
  void spool();

  void serial(const char *message);

  void setBusy(boolean _busyState);
  boolean isBusy();

  boolean home();
  boolean unlock();
  boolean reset();
  boolean pause();
  boolean resume();
  boolean status();
  boolean move(EventGrbl sens, float distance);
  boolean move(float x, float y, float z, int speed);
  boolean setXYZ(EventGrbl param);

  void jogMoveXY(float x, float y);
  void jogMoveZ(float z);

  // Event handler
  void notify(const Event *event);

  static GrblCtrl *instance();

  void getStoredMpos(float *x, float *y, float *z);
  void getStoredWpos(float *x, float *y, float *z);
  void getWorkingMpos(float *x, float *y, float *z);
  void getWorkingWpos(float *x, float *y, float *z);
  void getStoredModal(bool *metric, bool *abs);
  void getWorkingModal(bool *metric, bool *abs);

  void forceWrite(boolean flush, const char *grbl, ...);
  bool tryWrite(boolean flush, const char *grbl, ...);

protected:
  void error(const char *message);
  void alarm(const char *message);
  void status(const char *message);
  int available();
  int read();
  void write(boolean flush, const char *data);

  void flush();
  void decodeStatus(const char *, const char *);
  void decodeState(const char *, const char *);
  void decodeError(const char *, const char *);
  void decodeAlarm(const char *, const char *);
  void decodeOk(const char *, const char *);
  void decodeFeedback(const char *, const char *);

  void script(const char *input, char *output);
  void evaluate(const char *input, char *output, int sz);
  bool heartbeat();

private:
  // jog
  long lastJog = 0;
  float xyzJogPas = 0.1;
  float xyzL1Pas = 0.1;
  float xyzL2Pas = 0.5;
  float xyzL3Pas = 5;

  // this buffer is used to store a few char received from GRBL before decoding them
  char strGrblBuf[STR_GRBL_BUF_MAX_SIZE];
  // lower version of  strGrblBuf
  char strGrblBufNoCase[STR_GRBL_BUF_MAX_SIZE];
  char printBuffer[STR_GRBL_BUF_MAX_SIZE];
  char evalBuffer[STR_GRBL_BUF_MAX_SIZE];
  // write buffer
  char writeBuffer[STR_GRBL_BUF_MAX_WRITE_SIZE];
  // state
  GrblStatus grblState = GRBL_UNKNOWN;

  struct
  {
    struct
    {
      float x, y, z;
    } mpos;
    struct
    {
      float x, y, z;
    } wpos;
    boolean grblStatusMetric;
    boolean grblStatusAbs;
  } stored;

  struct
  {
    struct
    {
      float x, y, z;
    } mpos;
    struct
    {
      float x, y, z;
    } wpos;
    boolean grblStatusMetric;
    boolean grblStatusAbs;
  } working;

  uint8_t strGrblIdx;
  uint8_t txRead = 0;
  uint8_t txWrite = 0;
  uint8_t byteRead = 0;
  boolean busy = true;
  boolean isPrinting = false;
  int toPrintLines = 0;
  int printedLines = 0;
  boolean isPaused = false;
  GrblPrintStatus grblPrintStatus = empty;
  GrblStep step = M1;
  long lastStatus = 0;
  // only for simulation
  char internalSerialBuffer[MAXSIZE_OF_SIM];
  int internalSerialIndex = 0;
  int internalSerialCounter = 0;
  int uTime = 0;
};

#endif
