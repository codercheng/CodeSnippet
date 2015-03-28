#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "EasySock.h"


int main()
{

	int sockFd;
	CreateAndConn(sockFd, "127.0.0.1", 6868);

	SockSend sockSend;
	int iValue = 321;
	char cValue = 'k';
	long long int lValue = 9876543210;
	bool bValue = true;
	char cValue2 = 'T';

	sockSend.Appendint32(iValue);
	sockSend.AppendChar(cValue);
	sockSend.AppendInt64(lValue);
	sockSend.AppendBool(bValue);
	sockSend.AppendChar(cValue2);

	sockSend.SyncSend(sockFd);

	close(sockFd);
	printf("client exit successfully\n");
	return 0;
}

// g++ -g -Wall EasySock.cpp Client.cpp -o Client