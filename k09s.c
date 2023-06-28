#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <signal.h>
#include <errno.h>

#define DEF_PORT 7
#define BUF_SIZE 32

void SIGIOHandler(int sigType);
void excep(char *errMsg);

int SessionID;

int main(int argc, char *argv[])
{
	struct sockaddr_in ServerAddr;
	int svPort = DEF_PORT;
	struct sigaction handler;

	if(argc > 2){
		fprintf(stderr,"Usage %s <SERVER PORT>\n", argv[0]);
		exit(1);
	}
	else if(argc == 2){
		if((svPort = atoi(argv[1])) == 0){
			excep("Missing Port Number!\n");
		}
	}

	printf("Sever Port = %d\n",svPort);
	
	if ((SessionID=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP))==-1){
    		excep("Failed to make a socket!\n");
	}
	
	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port=htons(svPort);
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if (bind(SessionID,(struct sockaddr *) &ServerAddr,sizeof(ServerAddr))==-1){
		excep("Failed to assign a socketname.\n");
	}

	handler.sa_handler = SIGIOHandler;
	if(sigfillset(&handler.sa_mask) == -1){
		excep("sigfillset() Failed.\n");
	}
	handler.sa_flags = 0;
	
	if(sigaction(SIGIO, &handler, 0) == -1){
		excep("sigaction() Failed.");
	}
	
	if(fcntl(SessionID, F_SETOWN, getpid()) == -1){
		excep("fcntl() Failed : Unable to set process owner to us.");
	}
	
	if(fcntl(SessionID, F_SETFL, O_NONBLOCK | FASYNC) == -1){
		excep("fcntl() Failed : Unable to put client sock into nonblocking/async mode.");
	}
	
	for(;;){
		printf(".\n");
		sleep(3);
	}
		
}

void SIGIOHandler(int sigType)
{
	struct sockaddr_in ClientAddr;
	unsigned int clientlength;
	int recvSize;
	char msgBuffer[BUF_SIZE + 1];
	
	do{
		clientlength=sizeof(ClientAddr);
		
		if((recvSize = recvfrom(SessionID, msgBuffer, BUF_SIZE, 0, (struct sockaddr *) &ClientAddr, &clientlength)) == -1){
			if(errno != EWOULDBLOCK){
				excep("recvfrom() Failed.\n");
			}
		}
		else{
			msgBuffer[recvSize] = '\0';
			printf("HANDLING CLIENT : %s\n",inet_ntoa(ClientAddr.sin_addr));
			printf("RECIVED MASSAGE LENGTH: %d\n",recvSize);
			printf("RECIVED MASSAGE : %s\n\n",msgBuffer);
			
			/*if(sendto(SessionID, msgBuffer, recvSize, 0, (struct sockaddr *) &ClientAddr, sizeof(ClientAddr)) != recvSize){
				excep("sendto() error \n");
			}*/
		}
    } while (recvSize >= 0);
}

void excep(char *errMsg)
{
	fprintf(stderr,"%s",errMsg);
	exit(1);
}
