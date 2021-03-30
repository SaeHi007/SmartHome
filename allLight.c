#include "controldevice.h"

int allLightInit(int pin)
{
	pinMode(6,OUTPUT);
	pinMode(26,OUTPUT);
	pinMode(27,OUTPUT);
	pinMode(28,OUTPUT);
	pinMode(29,OUTPUT);
	
	digitalWrite(6,HIGH);
	digitalWrite(26,HIGH);
	digitalWrite(27,HIGH);
	digitalWrite(28,HIGH);
	digitalWrite(29,HIGH);
	return 1;
}

int allLightOpen(int pin)
{
	digitalWrite(6,LOW);
	digitalWrite(26,LOW);
	digitalWrite(27,LOW);
	digitalWrite(28,LOW);
	digitalWrite(29,LOW);
	return 1;
}

int allLightClose(int pin)
{
	digitalWrite(6,HIGH);
	digitalWrite(26,HIGH);
	digitalWrite(27,HIGH);
	digitalWrite(28,HIGH);
	digitalWrite(29,HIGH);
	return 1;
}

static struct CTRL_Device AllLight ={
	.initDevice=allLightInit,
	.open =allLightOpen,
	.close=allLightClose,
	.deviceName="allLight",
	.pin=0
};

struct CTRL_Device *addAllLightInLink(struct CTRL_Device *phead)
{
	if(phead == NULL){
		return &AllLight;
	}else{
		AllLight.next=phead;
		phead=&AllLight;
		return phead;
	}
	return phead;
}


