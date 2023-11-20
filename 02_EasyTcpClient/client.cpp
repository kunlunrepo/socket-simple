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
struct Login : public DataHeader // 继承
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
		else if (0 == strcmp(cmdBuf, "login")) {
			Login login; // 初始化结构体赋值
			strcpy(login.userName, "lyd"); // 字符串赋值
			strcpy(login.passWord, "lydxxxx");
			//DataHeader dh = {sizeof(login), CMD_LOGIN}; // 枚举从0开始
			//send(_sock, (const char *)&dh, sizeof(DataHeader), 0);
			send(_sock, (const char*)&login, sizeof(Login), 0);
			// 接收服务器返回的数据
			//DataHeader retHeader = {};
			LoginResult loginRet = {};
			//recv(_sock, (char*)&retHeader, sizeof(retHeader), 0);
			recv(_sock, (char*)&loginRet, sizeof(loginRet), 0);
			printf("LoginResult: %d \n", loginRet.result);
		}
		else if (0 == strcmp(cmdBuf, "logout")) {
			Logout logout;
			strcpy(logout.userName, "lyd");
			//DataHeader dh = { sizeof(logout), CMD_LOGOUT};
			// 
			//send(_sock, (const char *)&dh, sizeof(dh), 0);
			send(_sock, (const char *)&logout, sizeof(logout), 0);
			// 接收服务器返回的数据
			//DataHeader retHeader = {};
			LogoutResult logoutRet = {};
			//recv(_sock, (char *)&retHeader, sizeof(retHeader), 0);
			recv(_sock, (char *)&logoutRet, sizeof(logoutRet), 0);
			printf("LogoutResult: %d \n", logoutRet.result);

		}
		else
		{
			printf("不支持的命令，请重新输入\n");
		}
		// 6 接收服务器信息 recv
		/*char recvBuf[256] = {};
		int nlen = recv(_sock, recvBuf, 256, 0);
		if (nlen > 0)
		{
			DataPackage* dp = (DataPackage*)recvBuf;
			printf("接收到数据：年龄=%d 名称=%s \n", dp->age, dp->name);
		}*/
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