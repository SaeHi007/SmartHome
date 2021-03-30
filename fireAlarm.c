#include "controldevice.h"


int fireAlarmInit(int pin)
{
	pinMode(pin,INPUT);
	digitalWrite(pin,HIGH);
	return 1;
}

int fileAlarmReadStatus(int pin)
{
	return digitalRead(pin);
}
int fileAlarmChangeStatus(int pin)
{
	
}
static struct CTRL_Device fireAlarm ={
	.initDevice= fireAlarmInit,
	.readStatus= fileAlarmReadStatus,
	.changeStatus=fileAlarmChangeStatus,
	.deviceName="fileAlarm",
	.pin = 5
};

struct CTRL_Device *addFileAlarmInLink(struct CTRL_Device *phead)
{
	if(phead == NULL){
		return &fireAlarm;
	}else{
		fireAlarm.next=phead;
		phead=&fireAlarm;
		return phead;
	}
	return phead;
}



