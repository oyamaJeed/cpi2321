/*** clThread_12.c ***/

#include "myTCPSv.h"

/*** MAIN THREAD ***/
void *threadMain(void *thrArgs) {
	int clSock;	// Socket Descripter for Client
	
	pthread_detach(pthread_self());	// --> release resources
	
	/* ARGUMENT */
	clSock = ((struct threadArgs *)thrArgs)->clSock;
	free(thrArgs);
	
	/* HANDLE A CLIENT REQUEST */
	clHandler(clSock);
	return (NULL);
}


/*** HANDLE A CLIENT REQUEST ***/
void clHandler(int clSock)
{
	char buf[BUF_LEN];
	int bytesRcvd;
	char replyBuf[R_BUF_LEN];
	char *bufPtr;
	char *nextPtr;
	int i;
	char str[6];
	struct httpReqLine line = {NULL, NULL, NULL};	
	struct httpReqHeader head = {NULL, NULL};

	/* RECEIVE ONE LINE */
	bytesRcvd = recvOneLine(clSock, buf);
	pr_msg("recv():", buf);
	numToStr(bytesRcvd, str, 5);
	pr_msg("<size>:", str);

	line.method = strtok_re(buf, " \t\r\n", &nextPtr);
	pr_msg("method:", line.method);
	line.uri = strtok_re(NULL, " \t\r\n", &nextPtr);
	pr_msg("URI:", line.uri);
	line.httpVer = strtok_re(NULL, " \t\r\n", &nextPtr);
	pr_msg("HTTP_VER:", line.httpVer);

	for(i = 0; i < HEADERS_MAX;i++){
		bytesRcvd = recvOneLine(clSock, buf);
		//pr_msg("Hrecv():", buf);
		//numToStr(bytesRcvd, str, 5);
		//pr_msg("H<size>:", str);
		if(bytesRcvd == 0)break;
		
		numToStr(i, str, 3);
		pr_msg("\nHEAD No.",str);
		head.name = strtok_re(buf, ": \t\r\n", &bufPtr);
		pr_msg("HEAD.NAME:", head.name);
		head.val = strtok_re(NULL, " \t\r\n", &bufPtr);
		pr_msg("HEAD.VAR:", head.val);
	}

	
	/* CLOSE SOCKET */
	if (shutdown(clSock, 2) == -1) {
		excep("FAIL:shutdown()");
	}
	close(clSock);
	pr_msg("CONNECTION CLOSED.", "\n");
	return;
}

