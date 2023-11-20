#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 解决inet_ntoa

#include<windows.h>
#include<WinSock2.h>
#include<stdio.h>

enum CMD
{
	CMD_LOGIN,
	CMD_LOGIN_RESULT,
	CMD_LOGOUT,
	CMD_LOGOUT_RESULT,
	CMD_ERROR
};

struct DataHeader
{
	short dataLength;
	short cmd;
};

// 登录
struct Login: public DataHeader // 继承
{
	Login()
	{
		dataLength = sizeof(Login);
		cmd = CMD_LOGIN;
	}
	char userName[32];
	char passWord[32];
};
struct LoginResult : public DataHeader
{
	LoginResult()
	{
		dataLength = sizeof(LoginResult);
		cmd = CMD_LOGIN_RESULT;
		result = 0;
	}
	int result;
};

// 登出
struct Logout : public DataHeader
{
	Logout()
	{
		dataLength = sizeof(Logout);
		cmd = CMD_LOGOUT;
	}
	char userName[32];
};
struct LogoutResult : public DataHeader
{
	LogoutResult()
	{
		dataLength = sizeof(LogoutResult);
		cmd = CMD_LOGOUT_RESULT;
		result = 0;
	}
	int result;
};

int main()
{
	// 启动WindowsSocket2.x环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	// 启动socket函数
	WSAStartup(ver, &dat);

	// 1 建立一个socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// 2 bind 绑定用于接收客户端连接的端口
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567); // host to net unsigned short
	_sin.sin_addr.S_un.S_addr = INADDR_ANY; //inet_addr("127.0.0.1");
	if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_sin, sizeof(_sin)))
	{
		printf("错误, 绑定端口失败\n");
	} 
	else 
	{
		printf("绑定端口成功\n");
	}
	// 3 listen 监听网络端口
	if (SOCKET_ERROR == listen(_sock, 5))
	{
		printf("错误，监听端口失败\n");
	}
	else {
		printf("监听端口成功\n");
	}
	// 4 accept 等待接收客户端连接
	sockaddr_in clientAddr;
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET _cSock = INVALID_SOCKET;
	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (INVALID_SOCKET == _cSock)
	{
		printf("错误，接收到无效客户端SOCKET\n");
	}
	printf("新客户端加入：socket = %d IP = %s \n",(int)_cSock,  inet_ntoa(clientAddr.sin_addr));

	//char _recvBuf[128] = {};
	while (true) // 循环等待
	{
		DataHeader header = {};

		// 5 接收客户端数据
		int nLen = recv(_cSock, (char*)&header, sizeof(DataHeader), 0);
		if (nLen <= 0)
		{
			printf("客户端已退出，任务结束。\n");
			break;
		}
		
		switch (header.cmd)
		{
			case CMD_LOGIN:
				{
					Login login = {}; // 结构体重置
					recv(_cSock, (char*)&login + sizeof(DataHeader), sizeof(Login)-sizeof(DataHeader), 0);
					printf("收到命令：CMD_LOGIN 数据长度：%d,username=%s password=%s \n", login.dataLength, login.userName, login.passWord);
					LoginResult ret;
					//send(_cSock, (char*)&header, sizeof(DataHeader), 0); // 先发消息头
					send(_cSock, (char*)&ret, sizeof(LoginResult), 0); // 后发消息体
				}
				break;
			case CMD_LOGOUT:
				{
					Logout logout = {}; // 结构体重置
					recv(_cSock, (char*)&logout + sizeof(DataHeader), sizeof(Logout) - sizeof(DataHeader), 0);
					printf("收到命令：CMD_LOGIN 数据长度：%d,username=%s \n", logout.dataLength, logout.userName);
					LogoutResult ret;
					//send(_cSock, (char*)&header, sizeof(DataHeader), 0); // 先发消息头
					send(_cSock, (char*)&ret, sizeof(LogoutResult), 0); // 后发消息体
				}
				break;
			default:
				{
					header.cmd = CMD_ERROR;
					header.dataLength = 0;
					send(_cSock, (char*)&header, sizeof(DataHeader), 0); // 先发消息头

				}
				break;
		}
		
		// 6 处理请求
		//if (0 == strcmp(_recvBuf, "getInfo"))
		//{
		//	// 7 send 向客户端发送一条数据
		//	DataPackage dp = {80, "张国荣"};
		//	send(_cSock, (const char*)&dp, sizeof(DataPackage), 0);

		//}
		//else if (0 == strcmp(_recvBuf, "getAge"))
		//{
		//	// 7 send 向客户端发送一条数据
		//	char msgBuf[] = "80.";
		//	send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		//}
		//else
		//{
		//	// 7 send 向客户端发送一条数据
		//	char msgBuf[] = "???.";
		//	send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		//}
	}
	// 8 关闭套接字closeSocket
	closesocket(_sock);

	// 清除socket函数
	WSACleanup();
	printf("已退出,任务结束");
	getchar();
	return 0;
}