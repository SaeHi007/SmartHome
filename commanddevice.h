#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


struct InputCommander
{
	char commandName[128];
	char deviceName[128];
	char command[32];
	char IpAddress[128];
	char clientIpAddress[128];
	int device_fd1;
	int device_fd;
	long port;
	
	int (*deviceInit)(struct InputCommander *device,long baud);
	int (*getCommand)(struct InputCommander *device);
	int (*sendCommand)(struct InputCommander *device,char *cmd);
	int (*ohterFunction)(struct InputCommander *device);
	char log[1024];

	struct InputCommander *next;
};

struct CommandHandle{
	char deviceName[128];
	int  Cmd;
};


struct InputCommander *addVoiceControlCommandInLink(struct InputCommander *phead);
struct InputCommander *addSocketControlCommandInLink(struct InputCommander *phead);



