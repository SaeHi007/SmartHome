#include "commanddevice.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int socketInit(struct InputCommander *device,long baud)
{
	struct sockaddr_in s_addr;

	int bind_ret;
	int addrlen;
	memset(&s_addr,0,sizeof(struct sockaddr_in));
	
	//1.socket
	device->device_fd=socket(AF_INET,SOCK_STREAM,0);
	//2.bind
	s_addr.sin_family=AF_INET;
    s_addr.sin_port=htons(device->port);
	inet_aton(device->IpAddress,&s_addr.sin_addr);
	bind_ret=bind(device->device_fd,(struct sockaddr *)&s_addr,sizeof(struct sockaddr_in));
	
	//3.listen()
	listen(device->device_fd,10);
	
	return device->device_fd;
}

int socketAccept(struct InputCommander *device)
{
	struct sockaddr_in client_addr;
	int addrlen=0;
	int acceptStatus=0;
	memset(&client_addr,'\0',sizeof(struct sockaddr_in));
	addrlen=sizeof(struct sockaddr_in);
	device->device_fd1=accept(device->device_fd,(struct sockaddr *)& client_addr,&addrlen);
	
	strcpy(device->clientIpAddress,inet_ntoa(client_addr.sin_addr));
	return device->device_fd1;
}

int socketGetCommand(struct InputCommander *device)
{
	int nread=0;	
  	nread=read(device->device_fd1,device->command,sizeof(device->command));
	if(nread<0){
		perror("read");		
	}	
	return nread;
}

int socketSendCommand(struct InputCommander *device,char *cmd)
{
	int nwrite=0;
	nwrite=write(device->device_fd,cmd,strlen(cmd));
	if(nwrite == 0){
		perror("write");
	}
	return nwrite;
}


struct InputCommander SocketControl={
	.deviceInit= socketInit,
	.IpAddress="192.168.16.9",
	.port=8989,
	.commandName="SocketServer",
	.sendCommand=socketSendCommand,
	.getCommand =socketGetCommand,
	.ohterFunction=socketAccept,
	.command={'\0'},
	.clientIpAddress={'\0'},
	.log={'\0'}
};	

struct InputCommander *addSocketControlCommandInLink(struct InputCommander *phead)
{
	if(phead == NULL){
		return &SocketControl;
	}else{
		SocketControl.next=phead;
		phead=&SocketControl;
		return phead;
	}
	return phead;
}


