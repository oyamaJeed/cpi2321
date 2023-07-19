/*** k12s.c ***/
/* HTTP SERVER PART 1                 */
/*     Usage: ./k12s <server port> */
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
