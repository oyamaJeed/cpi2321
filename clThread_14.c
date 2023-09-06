/*** clThread_12.c ***/

#include "myTCPSv.h"

pthread_mutex_t flock = PTHREAD_MUTEX_INITIALIZER;

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

	if(strcmp(line.method, "GET")==0){
		pr_msg("GET GET","\n");
		sendHtml(clSock, line.uri);
	}

	/*for(i = 0; i < HEADERS_MAX;i++){
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
	}*/

	/* CLOSE SOCKET */
	if (shutdown(clSock, 2) == -1) {
		excep("FAIL:shutdown()");
	}
	close(clSock);
	//pr_msg("CONTENT--------", "\n");
	pr_msg("CONNECTION CLOSED.", "\n");
	return;
}

void sendHtml(int clSock, char *uri){
	FILE *file;
	char *uri_path;
	if(strcmp(uri, "/")==0){
		uri_path = "index.html";
	}
	else if(strcmp(uri, "/hello")==0){
		uri_path = "hello.html";
	}
	else if(strcmp(uri, "/k13")==0){
		uri_path = "k13.html";
	}
	else{
		uri_path = "404";
	}
	
	pthread_mutex_lock(&flock);
	file = fopen(uri_path, "r");

	if(file){	
		char msg_re[] = "HTTP/1.1 200 OK\nContent-Type: text/html; charset=utf-8\n\n";
		send(clSock, msg_re, sizeof(msg_re),0);
		char buff_re[1024];
		size_t bytes_re;
		while((bytes_re = fread(buff_re, 1, sizeof(buff_re), file)) > 0){
			send(clSock, buff_re,bytes_re,0);
		}	
		fclose(file);
	}
	pthread_mutex_unlock(&flock);

}
