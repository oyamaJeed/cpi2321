#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 7
#define BUF_SIZE 32
#define MAXPENDING 5

void *threadMain(void *arg);
int createTCPSvSock(unsigned short port);
int acceptTCPConn(int svSock);
void clHandler(int clSock);
void excep(char *errMsg);

struct ThreadArgs
{
	int clntsock;
};

int main(int argc, char *argv[])
{
	int ServerSock;
	int ClientSock;
	int svPort = PORT;
	pthread_t threadID;
	struct ThreadArgs *threadArgs;

	if(argc == 2){
		if((svPort = atoi(argv[1])) == 0){
			excep("Missing Port Number!\n");
		}
	}
	else if(argc > 2){
		excep("Usage: k10s [<Server Port>]\n");
	}
	
	ServerSock = createTCPSvSock(svPort);
	
	for(;;)
	{
		ClientSock = acceptTCPConn(ServerSock);
		
		if((threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) == NULL){
			excep("malloc() failed.\n");
		}
		threadArgs -> clntsock = ClientSock;
		
		if(pthread_create(&threadID, NULL, threadMain, (void *)threadArgs) != 0){
			excep("pthread_create() failed.\n");
		}
		printf("with thread %ld/n", (long int)threadID);
	}
}

void *threadMain(void *arg)
{
	int clntsock;
	pthread_detach(pthread_self());
	clntsock = ((struct ThreadArgs *) arg) -> clntsock;
	free(arg);
	
	clHandler(clntsock);
	
	return(NULL);
}


int createTCPSvSock(unsigned short port)
{
	int sock;
	struct sockaddr_in ServerAddr;
		
	if ((sock = socket(AF_INET,SOCK_STREAM,0))==-1){
		excep("Failed to make a socket!\n");
	}
	
	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ServerAddr.sin_port=htons(port);
	
	if (bind(sock,(struct sockaddr *) &ServerAddr,sizeof(ServerAddr))==-1){
		excep("bind() Failed\n");
	}
	
	if (listen(sock,MAXPENDING)==-1){
		excep("Failed to listen.\n");
	}
	
	return sock;
}

int acceptTCPConn(int svSock)
{
	int clntsock;
	struct sockaddr_in ClientAddr;
	int clientlength;
	
	clientlength=sizeof(ClientAddr);
	
	if ((clntsock=accept(svSock,(struct sockaddr *)&ClientAddr,(socklen_t *)&clientlength))==-1){
		excep("Failed to accept.\n");
	}
	printf("CONNECTED CLIENT : %s\n",inet_ntoa(ClientAddr.sin_addr));
	
	return clntsock;
}

void excep(char *errMsg)
{
	fprintf(stderr,"%s",errMsg);
	exit(1);
}

void clHandler(int clSock){
	int bytesRcvd;
	char buf[BUF_SIZE];
	if((bytesRcvd = recv(clSock,buf,BUF_SIZE, 0)) < 0){
		excep("recv() failed.\n");
	}
	while(bytesRcvd >0){
		if (send(clSock,buf,bytesRcvd,0)!=bytesRcvd){
			excep("Failed to send a message.\n");
		}
		
		if((bytesRcvd = recv(clSock,buf,BUF_SIZE, 0)) < 0){
			excep("recv() failed.\n");
		}	
		
	}
}
