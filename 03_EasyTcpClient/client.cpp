#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS // 解决inet_ntoa

#include<windows.h>
#include<WinSock2.h>
#include<stdio.h>

int main()
{
	// 启动WindowsSocket2.x环境
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	// 启动socket函数
	WSAStartup(ver, &dat);

	// 1 建立一个socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // SOCK_STREAM 面向数据流
	if (INVALID_SOCKET == _sock)
	{
		printf("错误，建立socket失败\n");
	}
	else 
	{
		printf("建立socket成功\n");
	}
	// 2 连接服务器 connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);// 端口
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // IP
	if (SOCKET_ERROR == connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in)))
	{
		printf("错误, 连接服务器失败\n");
	}
	else
	{
		printf("连接服务器成功\n");
	}
	
	while (true)
	{
		// 3 输入请求命令
		char cmdBuf[128] = {};
		scanf("%s", cmdBuf);
		// 4 处理请求命令
		if (0 == strcmp(cmdBuf, "exit"))
		{
			printf("收到退出命令\n");
			break;
		}
		else
		{
			// 5 向服务器发送请求命令
			send(_sock, cmdBuf, strlen(cmdBuf) + 1, 0);
		}
		// 6 接收服务器信息 recv
		char recvBuf[256] = {};
		int nlen = recv(_sock, recvBuf, 256, 0);
		if (nlen > 0)
		{
			printf("接收到数据：%s\n", recvBuf);
		}
	}

	
	// 7 关闭套接字closeSocket
	closesocket(_sock);
	getchar();
	// 清除socket函数
	WSACleanup();
	printf("已退出\n");
	getchar();
	return 0;
}