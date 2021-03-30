#include "controldevice.h"

int corridorLightInit(int pin)
{
	pinMode(pin,OUTPUT);
	digitalWrite(pin,HIGH);
	return 1;
}

int corridorLightOpen(int pin)
{
	digitalWrite(pin,LOW);
	return 1;
}

int corridorLightClose(int pin)
{
	digitalWrite(pin,HIGH);
	return 1;
}

static struct CTRL_Device corridorLight ={
	.initDevice=corridorLightInit,
	.open =corridorLightOpen,
	.close=corridorLightClose,
	.deviceName="corridorLiht",
	.pin = 26
};

struct CTRL_Device *addCorridorLightInLink(struct CTRL_Device *phead)
{
	if(phead == NULL){
		return &corridorLight;
	}else{
		corridorLight.next=phead;
		phead=&corridorLight;
		return phead;
	}
	return phead;
}

