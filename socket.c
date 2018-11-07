#include "socket.h"
#include <stdio.h>
#include <stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>

int conexion(){
	int socket_desc , client_sock , c;
	struct sockaddr_in server , client;
	reset = 0;

	//Crear socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("No se pudo crear el socket");
	}
	puts("Socket creado");
	int reuse = 1;
	if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0)
		perror("setsockopt(SO_REUSEADDR) fallo");

#ifdef SO_REUSEPORT
	if (setsockopt(socket_desc, SOL_SOCKET, SO_REUSEPORT, (const char*)&reuse, sizeof(reuse)) < 0)
		perror("setsockopt(SO_REUSEPORT) fallo");
#endif
	//Preparar la estructura de sockaddr_in
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 5000 );

	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("bind fallo. Error");
		return 1;
	}
	puts("bind hecho");

	//Listen
	listen(socket_desc , 3);

	//Aceptar conexiones entrantes
	puts("Esperando conexiones...");
	//Cargar interfaz Python, cliente
	system("sudo python interfaz.py&");
	c = sizeof(struct sockaddr_in);

	//Aceptar la conexion del cliente
	client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
	sock = client_sock;
	if (client_sock < 0)
	{
		perror("Fallo aceptando");
		return 1;
	}
	puts("Conexion aceptada");
	return 0;
}
void socket_writes(char message[1000]){
	if( send(sock , message , strlen(message), 0) < 0)
	{
		if (reset == 0) perror("Error enviando");
	}
}
char* socket_read(){
	int TotalByte;
	char message[512];
	char *result = (char *)malloc(strlen(message)+1);
	TotalByte = recv(sock , message , strlen(message), 0);
	if (TotalByte > 0){
		message[TotalByte] = '\0';
		strcpy(result,message);
	}
	else {
		strcpy(result,"procesando");
	}
	return result;
}
void socket_close(){
	reset = 1;
	socket_writes("[x]");
	close(sock);
}

