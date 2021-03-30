#include "controldevice.h"


int bedroomLightInit(int pin)
{
	pinMode(pin,OUTPUT);
	digitalWrite(pin,HIGH);
	return 1;
}

int  bedroomLightOpen(int pin)
{
	digitalWrite(pin,LOW);
	return 1;
}

int  bedroomLightClose(int pin)
{
	digitalWrite(pin,HIGH);
	return 1;
}

static struct CTRL_Device bedroomLight ={
	.initDevice= bedroomLightInit,
	.open =bedroomLightOpen,
	.close=bedroomLightClose,
	.deviceName="bedroomLiht",
	.pin = 29
};

struct CTRL_Device *addBedroomLightInLink(struct CTRL_Device *phead)
{
	if(phead == NULL){
		return &bedroomLight;
	}else{
		bedroomLight.next=phead;
		phead=&bedroomLight;
		return phead;
	}
	return phead;
}


