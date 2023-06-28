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
	struct sockaddr_in fromAddr;
	struct hostent *HostInfo;
	unsigned int svPort = DEF_PORT;
	unsigned int formSize;
	char *servIP;
	char *msgString;
	char msgBuffer[BUF_SIZE+1];
	int SessionID;
	int sendLen;
	int recvLen;

	if (argc < 3 || argc > 4){
	  fprintf(stderr,"Usage: %s <HostName> <Massage> [<Server Port>]\n",argv[0]);
	  exit(1);
	}

	servIP = argv[1];
	msgString = argv[2];
	
	sendLen = strlen(msgString);
	
	if(argc == 4){
		if((svPort = atoi(argv[3])) == 0){
			excep("Missing Port Number!\n");
		}
	}

	if ((SessionID=socket(PF_INET,SOCK_DGRAM,IPPROTO_UDP))==-1){
		excep("Failed to make a socket!\n");
	}
  
	memset(&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family=AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(servIP);
	ServerAddr.sin_port=htons(svPort);
		
	if(sendto(SessionID, msgString, sendLen, 0, (struct sockaddr *) &ServerAddr, sizeof(ServerAddr)) != sendLen){
		excep("sendto() Failed\n");
	}
	
	formSize = sizeof(fromAddr);
	if((recvLen = recvfrom(SessionID, msgBuffer, BUF_SIZE, 0, (struct sockaddr *) &fromAddr, &formSize)) == -1){
		excep("recvfrom() failed\n");
	}
	
	if(ServerAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr){
		excep("Message from Unknown source\n");
	}	
	
	msgBuffer[recvLen] = '\0';
	printf("ECHO-BACK MASSAGE : %s\n", msgBuffer);
	
	close(SessionID);
	exit(0);	
}

void excep(char *errMsg)
{
	fprintf(stderr,"%s",errMsg);
	exit(1);
}
