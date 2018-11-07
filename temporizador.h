#ifndef TEMPORIZADOR_H_
#define TEMPORIZADOR_H_

#include <time.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <wiringPi.h>

#define HEBRA 1

volatile int counter;
timer_t id_timer;

void timer_isr (union sigval arg);
void timer_start (int ms);
void timer_stop ();

#endif /*TEMPORIZADOR_H_*/
