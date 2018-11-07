#include "usuario.h"

struct user_list* create_list(char* name, char* id){
	struct user_list* new_list = (struct user_list*)malloc(sizeof(struct user_list));
	if(new_list == NULL){
		printf("Error.\n");
	}
	new_list->user.name = name;
	new_list->user.id = id;
	new_list->user.permission = "ADMIN"; //El primer usuario registrado por defecto es ADMIN
	new_list->next = NULL;

	return new_list;
}

void new_user(struct user_list* head, char* name, char* id, char* permission){
	struct user_list* user;
	struct user_list* cursor = head;

	user = (struct user_list*) malloc (sizeof(struct user_list));
	if (user == NULL) printf( "No hay memoria disponible!\n");

	user->user.name = name;
	user->user.id = id;
	user->user.permission = permission;

	while(cursor->next != NULL) cursor = cursor->next;
	cursor->next = user;
	user->next = NULL;
}

void deleteList(struct user_list** head){
	struct user_list* current = *head;
	struct user_list* next;

	while (current != NULL)
	{
		next = current->next;
		free(current);
		current = next;
	}
	*head = NULL;
}
