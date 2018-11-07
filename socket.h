#ifndef SOCKET_H_
#define SOCKET_H_


#include <stdio.h> //printf
#include <string.h>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr


int sock;
int reset;
int conexion();
void socket_writes(char message[1000]);
char* socket_read();
void socket_close();
#endif /*SOCKET_H_*/
