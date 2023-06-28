#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define BUF_SIZE 32
#define PORT 7


void excep(char *errMsg);

int main(int argc, char *argv[])
{
	struct sockaddr_in ServerAddr;
	struct hostent *HostInfo;
	int ServerSessionID;
	char sndBuffer[BUF_SIZE];
	char rcvBuffer[BUF_SIZE];
	int svPort = PORT;
	int sendLen;
	int bytesRcvd;
	int totalRcvd;
	int sndbytes;

	if (argc < 3 || argc > 4){
	  fprintf(stderr,"Usage: %s <HostName> <Massage> [<Server Port>]\n",argv[0]);
	  exit(1);
	}

	if(argc == 4){
		if((svPort = atoi(argv[3])) == 0){
			excep("Missing Port Number!\n");
		}
	}

	if ((ServerSessionID=socket(AF_INET,SOCK_STREAM,0))==-1){
		excep("Failed to make a socket!\n");
	}
  
	if ((HostInfo=gethostbyname(argv[1]))==NULL){
		excep("Failed to find host.\n");
	}


	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_port=htons(svPort);
	memcpy((char *)&ServerAddr.sin_addr,(char *)HostInfo->h_addr_list[0],HostInfo->h_length);
		
	if (connect(ServerSessionID,(struct sockaddr *) &ServerAddr,sizeof(ServerAddr))==-1){
		excep("Failed connect.\n");
	}
	sleep(1);
	
	sendLen = strlen(argv[2]);
	if (send(ServerSessionID,argv[2],sendLen,0)!=sendLen){
		  excep("Failed to send a message.\n");
		}
		
	totalRcvd = 0;
	while(totalRcvd < sendLen){
		if((bytesRcvd = recv(ServerSessionID,rcvBuffer,BUF_SIZE - 1, 0)) <= 0){
			excep("recv() failed.\n");
		}
		totalRcvd += bytesRcvd;
		rcvBuffer[bytesRcvd] = '\0';
		printf("%s",rcvBuffer);
	}
	printf("\n");
	close(ServerSessionID);
	exit(0);	
}

void excep(char *errMsg)
{
	fprintf(stderr,"%s",errMsg);
	exit(1);
}

