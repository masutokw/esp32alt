#include "conf.h"
#ifdef PAD
#include "pad.h"
#include "mount.h"
//#define PIN_MODE  13//16 // moved to conf.h
extern mount_t *telescope;
volatile int state[4] = {1, 1, 1, 1};
volatile int event[4] = {3, 3, 3, 3};
volatile long lastDebounceTime[4] = {0, 0, 0, 0};
const int debounceDelay = 10;
bool pad_enabled = false;
int bpin[4] = {PAD_N, PAD_S, PAD_W, PAD_E}; // defined in conf.h

void  IRAM_ATTR onChange(int b)
{
  int reading = digitalRead(bpin[b]);
  if (reading == state[b]) return;
  boolean debounce = false;
  if ((millis() - lastDebounceTime[b]) <= debounceDelay)
  {
    debounce = true;
  }
  lastDebounceTime[b] = millis();
  if (debounce) return;
  state[b] = reading;
  event[b] = state[b];
}
void  IRAM_ATTR onChange_North(void)
{
  onChange(0);
}
void  IRAM_ATTR onChange_South(void)
{
  onChange(1);
}
void  IRAM_ATTR onChange_West(void)
{
  onChange(2);
}
void  IRAM_ATTR onChange_East(void)
{
  onChange(3);
}
void doEvent(void)
{
  //int n;

  switch (event[0])
  {
    case  0:
      if (digitalRead(PIN_MODE)) mount_move(telescope, 'n');
      else telescope->srate = (telescope->srate + 1) % 4;
      event[0] = 3;
      break;
    case 1:
      mount_stop(telescope, 'n');
      event[0] = 3;
      break;
  }

  switch (event[1])
  {
    case  0:
      if (digitalRead(PIN_MODE)) mount_move(telescope, 's');
      else  telescope->srate = (telescope->srate + 5) % 4;
      event[1] = 3;
      break;
    case 1:
      mount_stop(telescope, 's');
      event[1] = 3;
      break;
  }
  switch (event[2])
  {
    case  0:
      mount_move(telescope, 'w');
      event[2] = 3;
      break;
    case 1:
      mount_stop(telescope, 'w');
      event[2] = 3;
      break;
  }

  switch (event[3])
  {
    case  0:
      mount_move(telescope, 'e');
      event[3] = 3;
      break;
    case 1:
      mount_stop(telescope, 'e');
      event[3] = 3;
      break;
  }

}

void pad_Init(void)
{
  pinMode(bpin[0], INPUT);
  pinMode(bpin[1], INPUT);
  pinMode(bpin[2], INPUT_PULLUP);
  pinMode(bpin[3], INPUT_PULLUP);
  pinMode(PIN_MODE, INPUT_PULLUP);
  attachInterrupt(bpin[0], onChange_North, CHANGE);
  attachInterrupt(bpin[1], onChange_South, CHANGE);
  attachInterrupt(bpin[2], onChange_West, CHANGE);
  attachInterrupt(bpin[3], onChange_East, CHANGE);
  pad_enabled = true;
}

void pad_Detach(void)
{
  detachInterrupt(bpin[0]);
  detachInterrupt(bpin[1]);
  detachInterrupt(bpin[2]);
  detachInterrupt(bpin[3]);
  pad_enabled = false;
}
#endif
