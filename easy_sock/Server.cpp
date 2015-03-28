//#include <ev.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "EasySock.h"

int listenFd;

int main()
{
	CreateAndBind(listenFd);

	int conn_fd;
 	struct sockaddr_in client_addr;
 	socklen_t len = sizeof(client_addr);

 	conn_fd = accept(listenFd, (struct sockaddr *)&client_addr, &len);
 	
	if(conn_fd > 0){
		 printf("got connection from ip:%s, port:%d\n", 
		 	inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port)); 

		 SockRecv sockRecv;
		 sockRecv.SyncRecv(conn_fd);

		 int iValue;
		 char cValue;
		 long long int lValue;
		 bool bValue;
		 char cValue2;

		 sockRecv.GetInt32(iValue);
		 sockRecv.GetChar(cValue);
		 sockRecv.GetInt64(lValue);
		 sockRecv.GetBool(bValue);
		 sockRecv.GetChar(cValue2);

		 printf("[%d:%c:%lld:%d:%c]\n", iValue, cValue, lValue, bValue, cValue2);
	}
	else if(conn_fd < 0){
		printf("connection error:%s ...\n", strerror(errno));
	}

	close(conn_fd);

	close(listenFd);
	printf("server exit successfully\n");

	return 0;
}
// g++ -g -Wall EasySock.cpp EasyScm.cpp -o Server