#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 7
#define BUF_SIZE 32

void clHandler(int clSock);
void excep(char *errMsg);

int main(int argc, char *argv[])
{
	struct sockaddr_in ServerAddr;
	struct sockaddr_in ClientAddr;
	struct hostent *HostInfo;
	int ServerSessionID;
	int ClientSessionID;
	int clientlength;
	int svPort = PORT;

	if(argc == 2){
		if((svPort = atoi(argv[1])) == 0){
			excep("Missing Port Number!\n");
			exit(1);
		}
	}
	printf("ServerPort = %d\n",svPort);
	
	if ((ServerSessionID=socket(AF_INET,SOCK_STREAM,0))==-1){
    		excep("Failed to make a socket!\n");
    		exit(1);
	}
	
	if ((HostInfo=gethostbyname("localhost"))==NULL){
    		excep("Failed to find host.\n");
    		exit(1);
	}
	
	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port=htons(svPort);
	//ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	memcpy((char *)&ServerAddr.sin_addr,(char *)HostInfo->h_addr_list[0],HostInfo->h_length);
	
	if (bind(ServerSessionID,(struct sockaddr *) &ServerAddr,sizeof(ServerAddr))==-1){
		excep("bind() Failed\n");
		exit(1);
	}
	
	if (listen(ServerSessionID,5)==-1){
		excep("Failed to listen.\n");
		exit(1);
	}
	
	while(1){
		clientlength=sizeof(ClientAddr);
		if ((ClientSessionID=accept(ServerSessionID,(struct sockaddr *)&ClientAddr,(socklen_t *)&clientlength))==-1){
			excep("Failed to accept.\n");
			exit(1);
		}
		printf("CONNECTED CLIENT : %s\n",inet_ntoa(ClientAddr.sin_addr));
		
		clHandler(ClientSessionID);
    }
}

void excep(char *errMsg)
{
	fprintf(stderr,"%s",errMsg);
}

void clHandler(int clSock){
	int bytesRcvd;
	char buf[BUF_SIZE];
	if((bytesRcvd = recv(clSock,buf,BUF_SIZE, 0)) < 0){
		excep("recv() failed.\n");
		exit(1);
	}
	while(bytesRcvd >0){
		if (send(clSock,buf,bytesRcvd,0)!=bytesRcvd){
			excep("Failed to send a message.\n");
			exit(1);
		}
		
		if((bytesRcvd = recv(clSock,buf,BUF_SIZE, 0)) < 0){
			excep("recv() failed.\n");
			exit(1);
		}	
		
	}
}
