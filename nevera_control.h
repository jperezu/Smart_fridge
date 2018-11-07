#ifndef NEVERA_CONTROL_H_
#define NEVERA_CONTROL_H_

#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <wiringPi.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

#include "fsm.h"
#include "usuario.h"
#include "temperatura.h"
#include "temporizador.h"
#include "inicializacion.h"
#include "rfid.h"
//Valores en segundos
#define TIME_INACTIVE 5
#define TIME_OPEN	10

char* access_id;

typedef enum bool {
	False = 0,
	True = 1,
}bool;

//Posibles estados de la máquina de estados
typedef enum fsm_state {
	CLOSED,
	UNLOCK,
	OPEN,
	ALARM,
	WAIT,
	LOCK_PIC,
}fridge_fsm_state_t;


typedef struct fridge_fsm_t{
	fsm_t fsm;			//Herencia
	struct user_list* user_list;
	bool alarm_en;
	bool stock_updated;
}fridge_fsm_t;

fridge_fsm_t* new_fridge_fsm(fsm_trans_t* fridge_transition_table, struct user_list* users);
void fsm_fridge_init (fridge_fsm_t* this, fsm_trans_t* fridge_transition_table, struct user_list* users);
int delete_fridge_fsm(fridge_fsm_t* fridge_fsm);

int check_id(fsm_t* this);
int time_exceed(fsm_t* this);
int door_state(fsm_t* this);
int data_processed(fsm_t* this);

void start_timer(fsm_t* this);
void change_lock(fsm_t* this);
void alarm_en(fsm_t* this);
void pic_take(fsm_t* this);
void pic_process(fsm_t* this);

#endif /* NEVERA_CONTROL_H_ */
