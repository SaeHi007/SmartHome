#include "controldevice.h"

int bathroomLightInit(int pin)
{
	pinMode(pin,OUTPUT);
	digitalWrite(pin,HIGH);
	return 1;
}

int bathroomLightOpen(int pin)
{
	digitalWrite(pin,LOW);
	return 1;
}

int bathroomLightClose(int pin)
{
	digitalWrite(pin,HIGH);
	return 1;
}

static struct CTRL_Device bathroomLight ={
	.initDevice=bathroomLightInit,
	.open =bathroomLightOpen,
	.close=bathroomLightClose,
	.deviceName="bathroomLiht",
	.pin = 28
};

struct CTRL_Device *addBathroomLightInLink(struct CTRL_Device *phead)
{
	if(phead == NULL){
		return &bathroomLight;
	}else{
		bathroomLight.next=phead;
		phead=&bathroomLight;
		return phead;
	}
	return phead;
}