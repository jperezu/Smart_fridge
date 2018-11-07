#include "inicializacion.h"

int initialize_sys(){
	int x;
	if (wiringPiSetupGpio () < 0) {
		printf ("Error configurando wiringPi\n");
		return -1;
	}

	pinMode (4, INPUT); //Sensor temperatura

	pinMode (PIN_RESET, INPUT); // BCM 23
	pinMode (PIN_DOOR, INPUT); // BCM 27
	pinMode (PIN_LOCK, OUTPUT); // BCM 17
	pinMode (PIN_ALARM, OUTPUT); // BCM 22
	pinMode (PIN_FLASH, OUTPUT); // BCM 21

	x = piThreadCreate (loop_temperature);

	if (x != 0) {
		printf ("Error iniciando la hebra\n");
		return -1;
	}
	return 1;
}
