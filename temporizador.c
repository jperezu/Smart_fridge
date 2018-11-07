#include "temporizador.h"

static int timer;

void timer_isr (union sigval arg) {
	timer = 1;
	piLock(HEBRA);
	counter++;
	piUnlock(HEBRA);

	printf("count = %04d\n", counter);
	fflush(stdout);
}

void timer_start (int ms){
	timer_t timerid;
	struct itimerspec spec;
	struct sigevent se;
	se.sigev_notify = SIGEV_THREAD;
	se.sigev_value.sival_ptr = &timerid;
	se.sigev_notify_function = timer_isr;
	se.sigev_notify_attributes = NULL;
	spec.it_value.tv_sec = ms / 1000;
	spec.it_value.tv_nsec = (ms % 1000) * 1000000;
	spec.it_interval.tv_sec = ms / 1000;
	spec.it_interval.tv_nsec = (ms % 1000) * 1000000;
	timer = 0;
	timer_create (CLOCK_REALTIME, &se, &timerid);
	timer_settime (timerid, 0, &spec, NULL);
	id_timer = timerid;
}
void timer_stop(){
	if (id_timer != NULL){
		timer_delete (id_timer);
	}
	piLock(HEBRA);
	counter = 0;
	piUnlock(HEBRA);
}
