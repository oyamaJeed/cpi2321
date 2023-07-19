/*** tcpip_12.c ***/

#include "myTCPSv.h"

/*** CREATE TCP SOCKET ***/
int createTCPSvSock(unsigned short port)
{
	int sock;					// Socket
	struct sockaddr_in svAddr;	// Local Address
	
	/* CREATE SOCKET */
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
		excep("FAIL:socket()");
	}

	/* INITIALIZE ADDRESS STRUCT. */
	memset(&svAddr, 0, sizeof(svAddr));
	svAddr.sin_family = AF_INET;
	svAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	svAddr.sin_port = htons(port);

	/* BIND (LOCAL SERVER ADDRESS) */
	if (bind(sock, (struct sockaddr *)&svAddr, sizeof(svAddr)) == -1) {
		excep("FAIL:bind()");
	}
	
	/* SET "LISTENING" */
	if (listen(sock, QUEUE_MAX) == -1) {
		excep("FAIL:listen()");
	}
	pr_msg("LISTENING:", "\n");
	
	return (sock);
}


/*** ACCEPT TCP CONNECTION ***/
int acceptTCPConn(int svSock)
{
	int clSock;					// Client Socket
	struct sockaddr_in clAddr;	// Remote(Client) Address
	unsigned int clAddrLen;		// Remote(Client) Address Struct. Size
	char addrStr[16];
	char portStr[6];
	
	clAddrLen = sizeof(clAddr);
	if ((clSock = accept(svSock, (struct sockaddr *)&clAddr,
											(socklen_t *)&clAddrLen)) == -1) {
		excep("FAIL:accept()");
	}
	inet_ntoa_re(clAddr.sin_addr, addrStr);
	pr_msg("CONNECTED CLIENT : ", addrStr);
	numToStr((long int)ntohs(clAddr.sin_port), portStr, 5);
	pr_msg("PORT : ", portStr);

	return (clSock);
}


/*** RECEIVE ONE LINE ***/
int recvOneLine(int clSock, char *buf)
{
	int dataSize;
	int bytesRcvd;
	
	dataSize = 0;
	while (1) {
		if ((bytesRcvd = recv(clSock, buf +dataSize, 1, 0)) == -1) {
			excep("FAIL:recv()[1]");
		}
		if (bytesRcvd == 0) break;
		dataSize++;
		if (*(buf +dataSize-1) == '\n') {
    			if (dataSize >= 2 && *(buf +dataSize-2) == '\r') {  // CR/LF
        			*(buf +dataSize-2) = '\0';
        			dataSize -= 2;
        			break;
      			}
			else {  // LF
				*(buf +dataSize-1) = '\0';
				dataSize--;
				break;
			}
    		}
		
		
	}
	return (dataSize);
}

