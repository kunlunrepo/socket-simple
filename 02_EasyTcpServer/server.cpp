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
struct Login: public DataHeader // �̳�
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
	SOCKET _sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// 2 bind �����ڽ��տͻ������ӵĶ˿�
	sockaddr_in _sin = {};
	_sin.sin_family = AF_INET;
	_sin.sin_port = htons(4567); // host to net unsigned short
	_sin.sin_addr.S_un.S_addr = INADDR_ANY; //inet_addr("127.0.0.1");
	if (SOCKET_ERROR == bind(_sock, (sockaddr*)&_sin, sizeof(_sin)))
	{
		printf("����, �󶨶˿�ʧ��\n");
	} 
	else 
	{
		printf("�󶨶˿ڳɹ�\n");
	}
	// 3 listen ��������˿�
	if (SOCKET_ERROR == listen(_sock, 5))
	{
		printf("���󣬼����˿�ʧ��\n");
	}
	else {
		printf("�����˿ڳɹ�\n");
	}
	// 4 accept �ȴ����տͻ�������
	sockaddr_in clientAddr;
	int nAddrLen = sizeof(sockaddr_in);
	SOCKET _cSock = INVALID_SOCKET;
	_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
	if (INVALID_SOCKET == _cSock)
	{
		printf("���󣬽��յ���Ч�ͻ���SOCKET\n");
	}
	printf("�¿ͻ��˼��룺socket = %d IP = %s \n",(int)_cSock,  inet_ntoa(clientAddr.sin_addr));

	//char _recvBuf[128] = {};
	while (true) // ѭ���ȴ�
	{
		DataHeader header = {};

		// 5 ���տͻ�������
		int nLen = recv(_cSock, (char*)&header, sizeof(DataHeader), 0);
		if (nLen <= 0)
		{
			printf("�ͻ������˳������������\n");
			break;
		}
		
		switch (header.cmd)
		{
			case CMD_LOGIN:
				{
					Login login = {}; // �ṹ������
					recv(_cSock, (char*)&login + sizeof(DataHeader), sizeof(Login)-sizeof(DataHeader), 0);
					printf("�յ����CMD_LOGIN ���ݳ��ȣ�%d,username=%s password=%s \n", login.dataLength, login.userName, login.passWord);
					LoginResult ret;
					//send(_cSock, (char*)&header, sizeof(DataHeader), 0); // �ȷ���Ϣͷ
					send(_cSock, (char*)&ret, sizeof(LoginResult), 0); // ����Ϣ��
				}
				break;
			case CMD_LOGOUT:
				{
					Logout logout = {}; // �ṹ������
					recv(_cSock, (char*)&logout + sizeof(DataHeader), sizeof(Logout) - sizeof(DataHeader), 0);
					printf("�յ����CMD_LOGIN ���ݳ��ȣ�%d,username=%s \n", logout.dataLength, logout.userName);
					LogoutResult ret;
					//send(_cSock, (char*)&header, sizeof(DataHeader), 0); // �ȷ���Ϣͷ
					send(_cSock, (char*)&ret, sizeof(LogoutResult), 0); // ����Ϣ��
				}
				break;
			default:
				{
					header.cmd = CMD_ERROR;
					header.dataLength = 0;
					send(_cSock, (char*)&header, sizeof(DataHeader), 0); // �ȷ���Ϣͷ

				}
				break;
		}
		
		// 6 ��������
		//if (0 == strcmp(_recvBuf, "getInfo"))
		//{
		//	// 7 send ��ͻ��˷���һ������
		//	DataPackage dp = {80, "�Ź���"};
		//	send(_cSock, (const char*)&dp, sizeof(DataPackage), 0);

		//}
		//else if (0 == strcmp(_recvBuf, "getAge"))
		//{
		//	// 7 send ��ͻ��˷���һ������
		//	char msgBuf[] = "80.";
		//	send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		//}
		//else
		//{
		//	// 7 send ��ͻ��˷���һ������
		//	char msgBuf[] = "???.";
		//	send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		//}
	}
	// 8 �ر��׽���closeSocket
	closesocket(_sock);

	// ���socket����
	WSACleanup();
	printf("���˳�,�������");
	getchar();
	return 0;
}