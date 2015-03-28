###easy_sock
  简化`数据发送和接收过程`。几行代码就完成数据的发送和传输，不需要去考虑socket里面具体的发送细节,不需要考虑发送的数据大小，不需要考虑TCP粘报等。目前先只支持同步，还有很多待完善。
  
  这个简单的来说，就是为了方便网络传输的，我们都知道rpc之类的，但是我很多东西完全没必要用rpc这种东西。
  在写easySCM这个项目的时候，我发现，我急需要一个简化数据传输通讯的库，因为这个项目涉及到很多小的数据传输，但是都是
  同一套路，有没有一个东西能简化这种通讯，不需要每次都考虑数据的大小，TCP粘包等。
  
  比如：
  
  client 发给服务端【int, char】,那么客户度只需要下面几行代码
  ``` c
  SockSend sockSend;
  
 	int iValue = 321;
 	char cValue = 'k';
 
 	sockSend.Appendint32(iValue);
 	sockSend.AppendChar(cValue);
  
   /*发送*/
 	sockSend.SyncSend(sockFd);
 
 	close(sockFd);
  ```
  server 接收客户度【int, char], 代码如下：
  ```c
  SockRecv sockRecv;
	 
	 /*接收*/
	 sockRecv.SyncRecv(conn_fd);

	 int iValue;
	 char cValue;

	 sockRecv.GetInt32(iValue);
	 sockRecv.GetChar(cValue);
	 /*Now,iValue=321; cValue='k'*/
	 close(conn_fd);
  ```
