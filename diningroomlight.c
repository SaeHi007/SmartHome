#include "controldevice.h"

int diningroomLightInit(int pin)
{
	pinMode(pin,OUTPUT);
	digitalWrite(pin,HIGH);
	return 1;
}

int diningroomLightOpen(int pin)
{
	digitalWrite(pin,LOW);
	return 1;
}

int diningroomLightClose(int pin)
{
	digitalWrite(pin,HIGH);
	return 1;
}

static struct CTRL_Device diningroomLight ={
	.initDevice=diningroomLightInit,
	.open =diningroomLightOpen,
	.close=diningroomLightClose,
	.deviceName="diningroomLiht",
	.pin = 27
};

struct CTRL_Device *addDiningroomLightInLink(struct CTRL_Device *phead)
{
	if(phead == NULL){
		return &diningroomLight;
	}else{
		diningroomLight.next=phead;
		phead=&diningroomLight;
		return phead;
	}
	return phead;
}

