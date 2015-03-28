#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>

#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "EasySock.h"

/*server*/
int CreateAndBind(int &listen_fd, int port, int backlog){

	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == listen_fd) {
		fprintf(stderr, "Cannot create sock\n");
		return -1;
	}

	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_port = htons(port);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	int ret;
	ret = bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if (-1 == ret) {
		//HANDLE_ERROR_EXIT("can not bind");
		fprintf(stderr, "Cannot Bind\n");
		return -1;
	}
	ret = listen(listen_fd, backlog);
	if (-1 == ret) {
		fprintf(stderr, "Cannot listen\n");
		return -1;
	}
	return 0;
}

/*client*/
int CreateAndConn(int &sock_fd, const char *ip, int port){
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (-1 == sock_fd) {
		fprintf(stderr, "cannot create sock\n");
		return -1;
	}
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(ip);

	int ret;
	ret = connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr));
	if (-1 == ret) {
		fprintf(stderr, "connect error\n");
		return -1;
	}
	return 0;
}

/******************** SockSend *************************/

SockSend::SockSend(int32_t iSize) {
	m_iLen = 4;
	m_iCapacity = m_iLen + iSize;
	m_pData = new unsigned char[m_iCapacity + 1];
}

SockSend::~SockSend() {
	delete[] m_pData;
}

void SockSend::Clear() {
	m_iLen = 4;
	memset(m_pData, 0, sizeof(m_pData));
}

void SockSend::AppendBool(const bool &bValue) {
	preCheckSize(m_iLen + 1);
	memmove(m_pData + m_iLen, &bValue, 1);
	m_iLen++;
}
void SockSend::Appendint32(const int32_t &iValue) {
	preCheckSize(m_iLen + 4);
	memmove(m_pData + m_iLen, &iValue, 4);
	m_iLen += 4;
}
void SockSend::AppendInt64(const int64_t &lValue) {
	preCheckSize(m_iLen + 8);
	memmove(m_pData + m_iLen, &lValue, 8);
	m_iLen += 8;
}
void SockSend::AppendChar(const char &cValue) {
	preCheckSize(m_iLen + 1);
	memmove(m_pData + m_iLen, &cValue, 1);
	m_iLen++;
}

int32_t SockSend::GetDataLength() {
	return m_iLen;
}
void SockSend::Test() {
	std::cout << "len:" << m_iLen << ", capacity:" << m_iCapacity << std::endl;
	std::cout << "data:" << m_pData[4] << std::endl;
	int32_t iTemp;
	memmove(&iTemp, &m_pData[5], 4);
	std::cout << "int:" << iTemp << std::endl;
}

//sync
int SockSend::SyncSend(int sockfd) {
	serialize();

	int iByteLeft = m_iLen;
	int pos = 0;

	while (iByteLeft) {
		int iByte;
		iByte = write(sockfd, m_pData + pos, iByteLeft);
		if (iByte > 0) {
			pos += iByte;
			iByteLeft -= iByte;
		}
		else if (iByte < 0) {
			fprintf(stderr, "%s", strerror(errno));
			return -1;
		}
		else {
			/*the other side quit*/
			close(sockfd);
			return -1;
		}
	}
	return 0;
}

void SockSend::serialize() {
	int32_t iDataLen = m_iLen - 4;
	memmove(m_pData, &iDataLen, 4);
	m_pData[m_iLen] = '\0';
}
void SockSend::preCheckSize(int iSize) {
	if (m_iCapacity >= iSize)
		return;
	int iNewCapacity = (2 * m_iCapacity) > (iSize) ? (2 * m_iCapacity) : (iSize);

	unsigned char *pTmpData = new unsigned char[iNewCapacity + 1];
	memmove(pTmpData, m_pData, m_iLen);
	delete[] m_pData;

	m_pData = pTmpData;
	m_iCapacity = iNewCapacity;
}


/***************** SockRecv *********************/

SockRecv::SockRecv() {
	m_pData = new unsigned char[4];
	m_iCurPos = 4;
	m_iLen = 4;
}
SockRecv::~SockRecv() {
	delete[] m_pData;
}
void SockRecv::Clear() {
	m_iCurPos = 4;
	m_iLen = 4;
}
int SockRecv::SyncRecv(int sockfd) {
	//recv the 4 bytes header first
	int iByteLeft = 4;
	int pos = 0;
	bool bVisited = false;
	while (iByteLeft) {
		int iByte;
		iByte = read(sockfd, m_pData + pos, iByteLeft);
		
		if (iByte > 0) {

			iByteLeft -= iByte;
			pos += iByte;
			if (!bVisited && iByteLeft <= 0) {//4 bytes header has been received
				
				memmove(&m_iLen, m_pData, 4);
				iByteLeft = m_iLen;

				m_iLen += 4;
				unsigned char *pTmpData = new unsigned char[m_iLen + 1];
				memmove(pTmpData, m_pData, 4);
				delete[] m_pData;

				m_pData = pTmpData;
				bVisited = true;
			}
		}
		else if (iByte < 0) {
			fprintf(stderr, "%s", strerror(errno));
			return -1;
		}
		else {
			close(sockfd);
			return -1;
		}
	}
	return 0;
}

void SockRecv::GetChar(char &cValue) {
	memmove(&cValue, m_pData + m_iCurPos, 1);
	m_iCurPos++;
}
void SockRecv::GetBool(bool &bValue) {
	memmove(&bValue, m_pData + m_iCurPos, 1);
	m_iCurPos++;
}
void SockRecv::GetInt32(int32_t &iValue) {
	memmove(&iValue, m_pData + m_iCurPos, 4);
	m_iCurPos += 4;
}
void SockRecv::GetInt64(int64_t &lValue) {
	memmove(&lValue, m_pData + m_iCurPos, 8);
	m_iCurPos += 8;
}


/*

int main()
{
	printf("hello world\n");
	SockSend sockData(1);

	char c = 'a';
	sockData.AppendChar(c);
	int32_t a = 32;
	sockData.Appendint32(a);

	//sockData.Serialize();
	sockData.Test();
	//cout<<sockData.GetDataLength()<<endl;

	return 0;
}
*/