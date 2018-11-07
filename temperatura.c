#include <String.h>
#include "temperatura.h"


char devPath[128]; // Path to device

void initialize_temp(){
	DIR *dir;
	struct dirent *dirent;
	char dev[16];      // Dev ID
	char path[] = "/sys/bus/w1/devices";

	dir = opendir (path);
	if (dir != NULL)
	{
		while ((dirent = readdir (dir)))
			// 1-wire devices are links beginning with 28-
			if (dirent->d_type == DT_LNK &&
					strstr(dirent->d_name, "28-") != NULL) {
				strcpy(dev, dirent->d_name);
				printf("\nSensor Temp: %s\n", dev);
			}
		(void) closedir (dir);
	}
	else
	{
		perror ("No se puede abrir el directorio de dispositivos w1");
	}

	sprintf(devPath, "%s/%s/w1_slave", path, dev);
}

void read_temperature(){
	char buf[256];     // Data
	char tmpData[6];   // Temp C * 1000
	ssize_t numRead;
	char temperatura [32];
	char temperatura_var [32];
	int fd = open(devPath, O_RDONLY);
	if(fd == -1)
	{
		perror ("No se puede abrir el sensor de temperatura.");
	}

	if ((numRead = read(fd, buf, 256)) > 0){
		strncpy(tmpData, strstr(buf, "t=")+2, 5);
		float tempC = strtof(tmpData, NULL);
		sprintf(temperatura, "%.2f", tempC / 1000);
		sprintf(temperatura_var,"%s%s","[0] ",temperatura);
		socket_writes(temperatura_var);
	}
	close(fd);
}


