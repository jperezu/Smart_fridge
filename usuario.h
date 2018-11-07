#ifndef USUARIO_H_
#define USUARIO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wiringPi.h>

typedef struct user{
	char* name;
	char* id;
	char* permission;
}user;

struct user_list {
  user user;
  struct user_list* next;
};

struct user_list* create_list(char* name, char* id);
void new_user(struct user_list* head, char* name, char* id, char* permission);
void deleteList(struct user_list** head);

#endif /*USUARIO_H_*/
