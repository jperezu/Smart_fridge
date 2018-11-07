#ifndef TEMPERATURA_H_
#define TEMPERATURA_H_

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "socket.h"
#include "nevera_control.h"
void initialize_temp();
void read_temperature();

#endif /*TEMPERATURA_H_*/
