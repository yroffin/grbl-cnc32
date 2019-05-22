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
#define GET_GRBL_STATUS_CLOSED 0
#define GET_GRBL_STATUS_START 1
#define GET_GRBL_STATUS_WPOS_HEADER 2
#define GET_GRBL_STATUS_WPOS_DATA 3
#define GET_GRBL_STATUS_HEADER 4
#define GET_GRBL_STATUS_F_DATA 5
#define GET_GRBL_STATUS_WCO_DATA 6
#define GET_GRBL_STATUS_ALARM 7
#define GET_GRBL_STATUS_BF_DATA 8
#define GET_GRBL_STATUS_MESSAGE 9

#define STR_GRBL_BUF_MAX_SIZE 256      // size has been increased from 10 to 50 to support grbl [Msg:]
#define STR_GRBL_BUF_MAX_WRITE_SIZE 64 // size has been increased from 10 to 50 to support grbl [Msg:]

enum GrblWay
{
  XP,
  XM,
  YP,
  YM,
  ZP,
  ZM,
  SETX,
  SETY,
  SETZ,
  SETXYZ
};

enum GrblPrintStatus
{
  empty,
  full
};

class GrblCtrl
{
public:
  GrblCtrl();

  void init();
  void capture();

  void print(const char *filename);
  void spool();

  boolean home();
  boolean unlock();
  boolean reset();
  boolean pause();
  boolean resume();
  boolean status();
  boolean move(GrblWay sens, float distance);
  boolean setXYZ(GrblWay param);

  // Event handler
  void notify(const Event *event);

  static GrblCtrl *instance();

protected:
  void error(const char *message);
  void alarm(const char *message);
  void status(const char *message);
  int available();
  int read();
  void flush();
  void decodeStatus(const char *, const char *);
  void decodeError(const char *, const char *);
  void decodeAlarm(const char *, const char *);
  void decodeOk(const char *, const char *);
  void decodeFeedback(const char *, const char *);

  void write(const char *grbl, ...);
  bool tryWrite(const char *grbl, ...);

private:
  // this buffer is used to store a few char received from GRBL before decoding them
  char strGrblBuf[STR_GRBL_BUF_MAX_SIZE];
  // lower version of  strGrblBuf
  char strGrblBufNoCase[STR_GRBL_BUF_MAX_SIZE];
  char printBuffer[STR_GRBL_BUF_MAX_SIZE];
  uint8_t strGrblIdx;
  uint8_t txRead = 0;
  uint8_t txWrite = 0;
  uint8_t byteRead = 0;
  boolean busy = true;
  boolean isPrinting = false;
  boolean isPaused = false;
  GrblPrintStatus grblPrintStatus = empty;
  // only for simulation
  char sim[512];
  char *idx;
  boolean simulation = true;
  long lastBusyWrite = 0;
};

#endif
