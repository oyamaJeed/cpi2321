/*** myTCPSv.h ***/
/* HEADER FILE for TCP/IP SERVER */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <arpa/inet.h>

#include <netdb.h>
#include <netinet/in.h>

#include <pthread.h>

#define DEBUG_K7 0
#define QUEUE_MAX 5
#define BUF_LEN 1024	// Receive Buffer Size
#define R_BUF_LEN 512	// Reply Buffer Size
#define HEADERS_MAX 20	// HTTP Headers 
#define MSG_LEN 128		// Console Message Buufer Size

/* FUNCTION PROTOTYPE */
//*** clThread
extern void *threadMain(void *thrArgs);					// Main Thread
extern void clHandler(int clSock);							// Handle A Client Request
extern void sendHtml(int clSock, char *uri);
//*** tcpip
extern int createTCPSvSock(unsigned short port);			// Create Socket
extern int acceptTCPConn(int svSock);						// Accept Connection
extern int recvOneLine(int clSock, char *buf);				// Receive One Line
//*** utils
extern void pr_msg(char *header, char *str);				// Print Message
extern void inet_ntoa_re(struct in_addr addr, char *str);	// 'inet_ntoa'(RE-ENTRANT)
extern void numToStr(long int num, char *str, int digit);	// Convert A Number To String
extern void excep(char *errMsg);							// Handle Exception
char *strtok_re(char *str, const char *delim, char **laterPart);	// 'strtok' (RE-ENTRANT)

extern void initSignal(void);								// Set Signal Handling

struct threadArgs {
	int clSock;	// Socket Descripter for Client
};

struct httpReqLine {
	char *method;
	char *uri;
	char *httpVer;
};

struct httpReqHeader {
	//int flag;
	char *name;
	char *val;
};
