#include "commanddevice.h"
#include <wiringSerial.h>

int uartVoiceInit(struct InputCommander *device,long baud)
{
	int uart_fd;
	wiringPiSetup();
	uart_fd=serialOpen(device->deviceName,baud);
	if(uart_fd==-1){
		printf("Seral open fail!\n");
		return 0;
	}else{
		device->device_fd=uart_fd;
		return uart_fd;
	}
}

int uartVoiceGetCmd(struct InputCommander *device)
{
 	int nread=0;
	nread=read(device->device_fd,device->command,sizeof(device->command));
	return nread;
}

struct InputCommander VoiceControlCommand={
	.deviceInit=uartVoiceInit,
	.commandName="UartDevice",
	.deviceName="/dev/ttyAMA0",
	.getCommand=uartVoiceGetCmd,
	.command={'\0'},
	.log={'\0'},
	.next=NULL
};
struct InputCommander *addVoiceControlCommandInLink(struct InputCommander *phead)
{
	if(phead == NULL){
		return &VoiceControlCommand;
	}else{
		VoiceControlCommand.next=phead;
		phead=&VoiceControlCommand;
		return phead;
	}
	return phead;
}

