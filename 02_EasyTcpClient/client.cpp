#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���inet_ntoa

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

// ��¼
struct Login : public DataHeader // �̳�
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

// �ǳ�
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
	// ����WindowsSocket2.x����
	WORD ver = MAKEWORD(2, 2);
	WSADATA dat;
	// ����socket����
	WSAStartup(ver, &dat);

	// 1 ����һ��socket
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // SOCK_STREAM ����������
	if (INVALID_SOCKET == _sock)
	{
		printf("���󣬽���socketʧ��\n");
	}
	else 
	{
		printf("����socket�ɹ�\n");
	}
	// 2 ���ӷ����� connect
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567);// �˿�
	_sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // IP
	if (SOCKET_ERROR == connect(_sock, (sockaddr*)&_sin, sizeof(sockaddr_in)))
	{
		printf("����, ���ӷ�����ʧ��\n");
	}
	else
	{
		printf("���ӷ������ɹ�\n");
	}
	
	while (true)
	{
		// 3 ������������
		char cmdBuf[128] = {};
		scanf("%s", cmdBuf);
		// 4 ������������
		if (0 == strcmp(cmdBuf, "exit"))
		{
			printf("�յ��˳�����\n");
			break;
		}
		else if (0 == strcmp(cmdBuf, "login")) {
			Login login; // ��ʼ���ṹ�帳ֵ
			strcpy(login.userName, "lyd"); // �ַ�����ֵ
			strcpy(login.passWord, "lydxxxx");
			//DataHeader dh = {sizeof(login), CMD_LOGIN}; // ö�ٴ�0��ʼ
			//send(_sock, (const char *)&dh, sizeof(DataHeader), 0);
			send(_sock, (const char*)&login, sizeof(Login), 0);
			// ���շ��������ص�����
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
			// ���շ��������ص�����
			//DataHeader retHeader = {};
			LogoutResult logoutRet = {};
			//recv(_sock, (char *)&retHeader, sizeof(retHeader), 0);
			recv(_sock, (char *)&logoutRet, sizeof(logoutRet), 0);
			printf("LogoutResult: %d \n", logoutRet.result);

		}
		else
		{
			printf("��֧�ֵ��������������\n");
		}
		// 6 ���շ�������Ϣ recv
		/*char recvBuf[256] = {};
		int nlen = recv(_sock, recvBuf, 256, 0);
		if (nlen > 0)
		{
			DataPackage* dp = (DataPackage*)recvBuf;
			printf("���յ����ݣ�����=%d ����=%s \n", dp->age, dp->name);
		}*/
	}

	
	// 7 �ر��׽���closeSocket
	closesocket(_sock);
	getchar();
	// ���socket����
	WSACleanup();
	printf("���˳�\n");
	getchar();
	return 0;
}