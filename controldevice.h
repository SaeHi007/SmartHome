#include <wiringPi.h>
#include <stdio.h>

struct CTRL_Device {
	char deviceName[128];
	int status;
	int pin;
	int (*initDevice)(int pinNum);
	int (*open)(int pinNum);
	int (*close)(int pinNum);
	int (*readStatus)(int pinNum);
	int (*changeStatus)(int Status);

	struct CTRL_Device *next;
};

struct CTRL_Device *addBathroomLightInLink(struct CTRL_Device *phead);
struct CTRL_Device *addBedroomLightInLink(struct CTRL_Device *phead);
struct CTRL_Device *addWimmingPoolLightInLink(struct CTRL_Device *phead);
struct CTRL_Device *addDiningroomLightInLink(struct CTRL_Device *phead);
struct CTRL_Device *addCorridorLightInLink(struct CTRL_Device *phead);
struct CTRL_Device *addFileAlarmInLink(struct CTRL_Device *phead);
struct CTRL_Device *addAllLightInLink(struct CTRL_Device *phead);


