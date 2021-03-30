#include "controldevice.h"

int swimmingPoolLightInit(int pin)
{
	pinMode(pin,OUTPUT);
	digitalWrite(pin,HIGH);
	return 1;
}

int swimmingPoolLightOpen(int pin)
{
	digitalWrite(pin,LOW);
	return 1;
}

int swimmingPoolLightClose(int pin)
{
	digitalWrite(pin,HIGH);
	return 1;
}

static struct CTRL_Device swimmingPoolLight ={
	.initDevice=swimmingPoolLightInit,
	.open =swimmingPoolLightOpen,
	.close=swimmingPoolLightClose,
	.deviceName="swimmingPoolLiht",
	.pin = 6
};

struct CTRL_Device *addWimmingPoolLightInLink(struct CTRL_Device *phead)
{
	if(phead == NULL){
		return &swimmingPoolLight;
	}else{
		swimmingPoolLight.next=phead;
		phead=&swimmingPoolLight;
		return phead;
	}
	return phead;
}

