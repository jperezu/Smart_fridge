#include "nevera_control.h"

// Máquina de estados: lista de transiciones
// { EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t  fridge_transition_table[] = {
		{CLOSED, check_id, UNLOCK, change_lock},
		{UNLOCK, time_exceed, CLOSED, change_lock},
		{UNLOCK, door_state, OPEN, start_timer},
		{OPEN, door_state, WAIT, start_timer},
		{OPEN, time_exceed, ALARM, alarm_en},
		{ALARM, door_state, WAIT, start_timer},
		{WAIT, door_state, OPEN, start_timer},
		{WAIT, time_exceed, LOCK_PIC, pic_take},
		{LOCK_PIC, data_processed, CLOSED, pic_process},
		{-1, NULL, -1, NULL },
};

fridge_fsm_t* new_fridge_fsm(fsm_trans_t* tt, struct user_list* users){
	fridge_fsm_t* this = (fridge_fsm_t*) malloc(sizeof(fridge_fsm_t));

	if(this != NULL){
		fsm_fridge_init (this, tt, users);
	}
	return this;
}

void fsm_fridge_init (fridge_fsm_t* this, fsm_trans_t* fridge_transition_table, struct user_list* users){
	this->fsm.current_state = CLOSED;
	this->fsm.tt = fridge_transition_table;
	this->user_list = users;
	this->alarm_en = False;
	this->stock_updated = False;

	digitalWrite(PIN_LOCK, HIGH);
}

int delete_fridge_fsm(fridge_fsm_t* fridge_fsm){
	free(fridge_fsm);
	return 1;
}

///////TABLA DE FUNCIONES//////
//FUNCIONES DE CONDICION

//Condiciones para entrar en un estado
int check_id(fsm_t* this){
	int i;
	fridge_fsm_t* fridge = (fridge_fsm_t*) this;
	struct user_list* cursor =fridge->user_list;
	char usuario_var[100];
	while(cursor != NULL){
		if(strcmp(cursor->user.id, access_id) == 0){
			sprintf(usuario_var,"%s%s","[2] ",cursor->user.name);
			socket_writes(usuario_var);
			for (i=0;i<2;i++){
				digitalWrite(PIN_ALARM, HIGH);
				delay(100);
				digitalWrite(PIN_ALARM, LOW);
				delay(100);
			}
			return 1;
		}
		else if (strcmp(access_id, "00000000") != 0){
			printf("Acceso no autorizado\n");
			socket_writes("[2] Acceso no autorizado");
			digitalWrite(PIN_ALARM, HIGH);
			delay(1500);
			digitalWrite(PIN_ALARM, LOW);
		}
		cursor = cursor->next;
	}
	return 0;
}

//Controla  los tiempos para cambiar de estado entre:
//UNLOCK->CLOSED
//OPEN->ALARM
//WAIT->LOCK
int time_exceed(fsm_t* this){
	switch(this->current_state)
	{
	case UNLOCK:
		if (counter == TIME_INACTIVE) return 1;
		break;
	case OPEN:
		if (counter == TIME_OPEN) return 1;
		break;
	case WAIT:
		if (counter == TIME_INACTIVE) return 1;
		break;
	default :
		return 0;
	}
	return 0;

}
//Comprueba si la puerta esta abierta o cerrada
// Si puerta abierta: UNLOCK->OPEN WAIT ->OPEN
// Si puerta cerrada: OPEN->WAIT ALARM->WAIT
int door_state(fsm_t* this){
	if ((this->current_state == OPEN) || (this->current_state == ALARM) )
		return digitalRead(PIN_DOOR);
	return !digitalRead(PIN_DOOR);
}

//Si la informacion se ha procesado vuelve al estado de reposo cerrado
int data_processed(fsm_t* this){
	fridge_fsm_t* fridge = (fridge_fsm_t*) this;
	char filename[] = "inventario.txt";
	char timeStr[ 100 ] = "";
	struct stat buf;
	time_t ltime;

	if (!stat(filename, &buf))
	{
		strftime(timeStr, 100, "%d-%m-%Y %H:%M:%S", localtime( &buf.st_mtime));
	}
	else
	{
		printf("error obteniendo la hora\n");
	}
	time( &ltime );
	if (difftime(ltime ,buf.st_mtime ) == 0 )
		fridge->stock_updated = True;
	return fridge->stock_updated;
}


//FUNCIONES DE SALIDA

//Implementación de funciones OUTPUT de la máquina de estados.
//Se ejecutan cuando se alcanza el estado.


//Activa el contador considerar un temporizador de POSIX
//Apagar alarma (GPIO-ALARMA --> LOW)
void start_timer(fsm_t* this){
	fridge_fsm_t* fridge = (fridge_fsm_t*) this;
	timer_stop();
	timer_start(1000);
	if (fridge->alarm_en){
		fridge->alarm_en = False;
		digitalWrite(PIN_ALARM, LOW);
		printf("ALARMA DESACTIVADA\n");
	}
}

//Conmuta el cierre entre abierto o cerrado
//GPIO-CIERRE --> !estado
void change_lock(fsm_t* this){
	int lock;

	timer_stop();
	if (this->current_state == UNLOCK) start_timer(this);
	lock = digitalRead(PIN_LOCK);
	digitalWrite(PIN_LOCK,!lock);
}

//Activación de la señal de alarma, GPIO
void alarm_en(fsm_t* this){
	fridge_fsm_t* fridge = (fridge_fsm_t*) this;

	timer_stop();
	if (!(fridge->alarm_en)){
		fridge->alarm_en = True;
		digitalWrite(PIN_ALARM, HIGH);
		printf("ALARMA ACTIVADA\n");
	}
}
void pic_take(fsm_t* this){
	change_lock(this);
	digitalWrite(PIN_FLASH, HIGH);
	printf("Iniciando camara...\n");
	system("raspistill -n -o image1.jpg");
	printf("Interior capturado\n");
	digitalWrite(PIN_FLASH, LOW);
}

//analisis de la info
void pic_process(fsm_t* this){
	printf("Inventario actualizado\n");
	fridge_fsm_t* fridge = (fridge_fsm_t*) this;
	fridge->stock_updated = False;
}

