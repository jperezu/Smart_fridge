#include "nevera_control.h"
#include "inicializacion.h"
#include "rfid.h"
#include "usuario.h"
#include "temperatura.h"
#include "socket.h"

#define CLK_MS 	100
extern fsm_trans_t fridge_transition_table[];

void delay_until (unsigned int next) {
	unsigned int now = millis();

	if (next > now) {
		delay (next - now);
	}
}

PI_THREAD (loop_temperature) {
	while(1) {
		delay(1000);
		read_temperature();
	}
}

int main (){
	char* states[] = {"CERRADA", "DESBLOQUEADA", "ABIERTA", "ALARMA", "ESPERANDO", "PROCESANDO"};
	int last_state = 0;
	char puerta[100];

	// Configuracion e inicializacion del sistema
	unsigned int next;
	struct user_list* users;

	conexion();
	initialize_temp();
	initialize_rfid();
	initialize_sys();

	next = millis();
	users = create_list("Jorge", "502D897C");
	fsm_t* fsm = (fsm_t*) new_fridge_fsm(fridge_transition_table, users);

	while (1) {
		if (fsm->current_state == 0) {
			access_id = read_id();
			socket_writes("[2] ");
		}

		fsm_fire (fsm);
		next += CLK_MS;
		delay_until (next);

		if(digitalRead(PIN_RESET)) break;

		if (fsm->current_state != last_state){
			last_state = fsm->current_state;
			printf("ESTADO: %s\n", states[fsm->current_state]);
			sprintf(puerta, "%s%s","[1] ",states[fsm->current_state]);
			socket_writes(puerta);
		}
	}

	socket_close();
	digitalWrite(PIN_LOCK, LOW);
	digitalWrite(PIN_ALARM, LOW);
	digitalWrite(PIN_FLASH, LOW);
	deleteList(&users);
	delete_fridge_fsm((fridge_fsm_t*) fsm);

	delay(100);
	printf("Sistema reseteado\n");
	return 0;
}

