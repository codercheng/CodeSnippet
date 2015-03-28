#ifndef _EASY_SOCK_H
#define _EASY_SOCK_H

#include <stdint.h>

class SockSend {
public:
	SockSend(int32_t iSize = 64);
	~SockSend();

/***********************************************************/
/***********************************************************/
/***/											
/***/	void Clear();							
/***/
/***/	void Appendint32(const int32_t &iValue);
/***/	void AppendInt64(const int64_t &lValue);
/***/	void AppendChar(const char &cValue);
/***/	void AppendBool(const bool &bValue);
/***/
/***/	int SyncSend(int sockfd);
/***/
/**********************************************************/
/**********************************************************/

	/*for test*/
	int32_t GetDataLength();
	void Test();

private:
	void serialize();
	void preCheckSize(int32_t iSize);

private:
	unsigned char *m_pData;
	int32_t m_iLen;
	int32_t m_iCapacity;
};

class SockRecv {
public:
	SockRecv();
	~SockRecv();

/***********************************************************/
/***********************************************************/
/***/
/***/	void Clear();
/***/
/***/	void GetInt32(int32_t &iValue);
/***/	void GetInt64(int64_t &lValue);
/***/	void GetChar(char &cValue);
/***/	void GetBool(bool &bValue);
/***/
/***/	int SyncRecv(int sockfd);
/***/
/***********************************************************/
/***********************************************************/
private:
	unsigned char *m_pData;
	int32_t m_iLen;
	int32_t m_iCurPos;//for parsing
};


/***********************************************************/
/***********************************************************/
extern 
int CreateAndBind(int &listen_fd, int port = 6868, int backlog = 5);
extern
int CreateAndConn(int &sock_fd, const char *ip, int port);

/***********************************************************/
/***********************************************************/

#endif