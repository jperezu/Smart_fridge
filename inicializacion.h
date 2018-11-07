#ifndef INICIALIZACION_H_
#define INICIALIZACION_H_

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>

//GPIOs BCM
static const int PIN_LOCK = 17;
static const int PIN_DOOR = 27;
static const int PIN_ALARM = 22;
static const int PIN_RESET = 23;
static const int PIN_FLASH = 21;

int initialize_sys();
PI_THREAD (loop_temperature);

#endif /*INICIALIZACION_H_*/
