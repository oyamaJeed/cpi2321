#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define DEF_PORT 7
#define BUF_SIZE 32

void excep(char *errMsg);

int main(int argc, char *argv[])
{
	struct sockaddr_in ServerAddr;
	struct sockaddr_in ClientAddr;
	char msgBuffer[BUF_SIZE + 1];
	int SessionID;
	unsigned int clientlength;
	int recvSize;
	int svPort = DEF_PORT;

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

	while(1){
		clientlength=sizeof(ClientAddr);
		
		if((recvSize = recvfrom(SessionID, msgBuffer, BUF_SIZE, 0, (struct sockaddr *) &ClientAddr, &clientlength)) == -1){
			excep("recvfrom() Failed.\n");
		}
	
		msgBuffer[recvSize] = '\0';
		printf("HANDLING CLIENT : %s\n",inet_ntoa(ClientAddr.sin_addr));
		printf("RECIVED MASSAGE LENGTH: %d\n",recvSize);
		printf("RECIVED MASSAGE : %s\n\n",msgBuffer);
		
		if(sendto(SessionID, msgBuffer, recvSize, 0, (struct sockaddr *) &ClientAddr, sizeof(ClientAddr)) != recvSize){
			excep("sendto() error \n");
		}
    }
}

void excep(char *errMsg)
{
	fprintf(stderr,"%s",errMsg);
	exit(1);
}
