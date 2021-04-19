#ifndef SLEEP_H_INCLUDED
#define SLEEP_H_INCLUDED

#include <Arduino.h>
#include "common.h"

void goToSleep(int secondsToSleep = CFG_SLEEP_SECONDS);

#endif