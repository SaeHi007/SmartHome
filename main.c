#include <stdio.h>
#include "commanddevice.h"
#include "controldevice.h"
#include <string.h>
#include <pthread.h>
#define Debug 1

#define openLight 	1
#define	closeLight  2


struct InputCommander *cmdPhead=NULL;
struct CTRL_Device *phead=NULL;
struct InputCommander *socketHandle=NULL;
pthread_mutex_t mutex;

struct CTRL_Device *queryDeviceLink(struct CTRL_Device *phead,char *str)
{
	struct CTRL_Device *temp=phead;
	if(phead==NULL){
		return NULL;
	}else{
		while(temp !=NULL){
			if(!strcmp( temp->deviceName,str)){
				return temp;
			}
			temp=temp -> next;
		}
	}
}
struct InputCommander *queryCommandLink(struct InputCommander *phead,char *str)
{
	struct InputCommander *temp=phead;
	if(phead==NULL){
		return NULL;
	}else{
		while(temp !=NULL){
			if(!strcmp( temp->commandName,str)){
				return temp;
			}
			temp=temp -> next;
		}
	}
}


void voiceCommandHandleFun(struct CommandHandle *commandhandle,char *cmd)
{	
	memset(commandhandle->deviceName,'\0',sizeof(commandhandle->deviceName));
//"diningroomLiht"
	if( !strcmp(cmd,"DL ON")|| !strcmp(cmd,"DL OFF")){
		strcpy(commandhandle->deviceName,"diningroomLiht");
		if(!strcmp(cmd,"DL ON")){commandhandle->Cmd=openLight;}
		else {commandhandle->Cmd=closeLight;}		
	}
//"bathroomLiht"
	if(!strcmp(cmd,"BL ON")||!strcmp(cmd,"BL OFF")){
		strcpy(commandhandle->deviceName,"bathroomLiht");	
		if(!strcmp(cmd,"BL ON")){ commandhandle->Cmd=openLight;}
		else {commandhandle->Cmd=closeLight;}
	}
//	"corridorLiht"
	if(!strcmp(cmd,"CL ON")||!strcmp(cmd,"CL OFF")){
		strcpy(commandhandle->deviceName,"corridorLiht");
		
		if(!strcmp(cmd,"CL ON")){commandhandle->Cmd=openLight;} 
		else {commandhandle->Cmd=closeLight;}
	}
	
//	"bedroomLiht"	
	if(!strcmp(cmd,"BDL ON")||!strcmp(cmd,"BDL OFF")){
		 	strcpy(commandhandle->deviceName,"bedroomLiht");
			
		 	if(!strcmp(cmd,"BDL ON")){commandhandle->Cmd=openLight;} 
			else {commandhandle->Cmd=closeLight;}
		}
//	"swimmingPoolLiht"
	if(!strcmp(cmd,"SL ON")||!strcmp(cmd,"SL OFF")){
			strcpy(commandhandle->deviceName,"swimmingPoolLiht");
			
			if(!strcmp(cmd,"SL ON")){commandhandle->Cmd=openLight;} 
			else{commandhandle->Cmd=closeLight;} 
		}
// "allLight"
	if(!strcmp(cmd,"All ON")||!strcmp(cmd,"All OFF")){
			strcpy(commandhandle->deviceName,"allLight");

			if(!strcmp(cmd,"All ON")){commandhandle->Cmd=openLight;} 
			else{ commandhandle->Cmd=closeLight;}
		}
	//commandhandle=&commandhandle1;
	

}

void *Voice_Routine(void *arg)
{
	struct InputCommander *temp=NULL;
	struct CTRL_Device *ctrl_device=NULL;
	struct CommandHandle command_hanle;
	
	int nread=0;

	temp=queryCommandLink(cmdPhead,"UartDevice");
	if(temp==NULL){
		printf("NULL\n");
		pthread_exit(NULL);
	}else{
		if(temp->deviceInit(temp,115200)<0){
			pthread_exit(NULL);
		}
		pthread_mutex_lock(&mutex);
		while(1){
			memset(temp->command,'\0',sizeof(temp->command));
			
			nread=temp->getCommand(temp);
		
			if(nread == 0 ){
				printf("Not voice data!\n");
			}else{
			#if Debug
				printf("voice command:%s\n",temp->command);
			#endif
			
				voiceCommandHandleFun(&command_hanle,temp->command);
			
			#if Debug
				printf("voice command name:%s\n",command_hanle.deviceName);
				printf("void comand num:%d\n",command_hanle.Cmd);
			#endif
			
				ctrl_device=queryDeviceLink(phead,command_hanle.deviceName);
			
			#if Debug
				printf("find device %s\n",ctrl_device->deviceName);
			#endif
			
				if(ctrl_device!=NULL){
					ctrl_device->initDevice(ctrl_device->pin);
					if(command_hanle.Cmd == openLight)		 ctrl_device->open(ctrl_device->pin);
					else if(command_hanle.Cmd == closeLight) ctrl_device->close(ctrl_device->pin);
				}
			}
			pthread_mutex_unlock(&mutex);
			//pthread_exit(NULL);
		}
		
	}
	
}

void *Read_Routine(void *arg)
{

	int nread=0;
	struct CommandHandle commandHandle;
	struct CTRL_Device *ctrl_device=NULL;
	
	pthread_mutex_lock(&mutex);
	#if Debug 
			printf("socket lock sucess!\n");
	#endif
	while(1){
		memset(socketHandle->command,'\0',sizeof(socketHandle->command));
		nread=socketHandle->getCommand(socketHandle);
		if(nread==0){
			printf("Client %s exit!\n",socketHandle->clientIpAddress);
			pthread_mutex_unlock(&mutex);
			pthread_exit(NULL);
		}
		printf("%s:%s\n",socketHandle->clientIpAddress,socketHandle->command);
		
		voiceCommandHandleFun(&commandHandle,socketHandle->command);
		ctrl_device=queryDeviceLink(phead,commandHandle.deviceName);
		
		if(ctrl_device!=NULL){
			ctrl_device->initDevice(ctrl_device->pin);
			if(commandHandle.Cmd == openLight)		 ctrl_device->open(ctrl_device->pin);
			else if(commandHandle.Cmd == closeLight) ctrl_device->close(ctrl_device->pin);
		}
		
		pthread_mutex_unlock(&mutex);
	}
  	
}

void *Socke_Routine(void *arg)
{
	int addrlen=0;
	pthread_t readThread;
	socketHandle=queryCommandLink(cmdPhead,"SocketServer");
	if(socketHandle == NULL){
		printf("Find socketHandle error!\n");
		pthread_exit(NULL);
	}else{
		socketHandle->deviceInit(socketHandle,0);

		#if Debug 
			printf("socket create sucess!\n");
		#endif
		
		while(1){	
			if(socketHandle->ohterFunction(socketHandle)){
				printf("client IP:%s\n",socketHandle->clientIpAddress);
				pthread_create(&readThread,NULL,Read_Routine,NULL);
			}
		//	pthread_exit(NULL);
		}
	}
	
}


int main()
{
	
	struct CTRL_Device *Device=NULL;
	
	pthread_t voice_thread;
	pthread_t socket_thread;
	int cmd=0;
	char deviceName[128]={0};
	
	if(wiringPiSetup()==-1){
		printf("Pi Setup fail!\n");
		return 0;
	}
	//device factory
	phead=addBathroomLightInLink(phead);
	phead=addBedroomLightInLink(phead);
	phead=addCorridorLightInLink(phead);
	phead=addDiningroomLightInLink(phead);
	phead=addWimmingPoolLightInLink(phead);
	phead=addAllLightInLink(phead);
	phead=addFileAlarmInLink(phead);

    //command factory
	cmdPhead=addVoiceControlCommandInLink(cmdPhead);
	cmdPhead=addSocketControlCommandInLink(cmdPhead);
	
	printf("bathroomLiht\n");
	printf("bedroomLiht\n");
	printf("corridorLiht\n");
	printf("diningroomLiht\n");
	printf("swimmingPoolLiht\n");

	//voice thread
	
	/*int pthread_create(pthread_t *thread, const pthread_attr_t *attr,void *(*start_routine) (void *), void *arg);*/
	pthread_create(&voice_thread,NULL,Voice_Routine,NULL);
	
	//socket thread
	pthread_create(&socket_thread,NULL,Socke_Routine,NULL);

	pthread_mutex_init(&mutex,NULL);

	//pthread_join(socket_thread,NULL);
	//phtread_join(voice_thread,NULL);
	while(1){
		printf("input device name:");
		scanf("%s",deviceName);
		getchar();
		Device=queryDeviceLink(phead,deviceName);
		if(Device!=NULL){
			Device->initDevice(Device->pin);
			printf("input cmd(1/0:1 is open;0 is close):");
			scanf("%d",&cmd);
			
			switch(cmd){
				case 1:Device->open(Device->pin);printf("open sucess!\n");break;
				case 0:Device->close(Device->pin);printf("close sucess!\n");break;
			}
			
		}
		memset(deviceName,'\0',sizeof(deviceName));
	}
	pthread_mutex_destroy(&mutex);
	pinMode(6,INPUT);
	pinMode(26,INPUT);
	pinMode(27,INPUT);
	pinMode(28,INPUT);
	pinMode(29,INPUT);
	return 0;
}
