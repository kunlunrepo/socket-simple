#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���inet_ntoa

#include<windows.h>
#include<WinSock2.h>
#include<stdio.h>

#include <vector>

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

std::vector<SOCKET> g_clients;

int processor(SOCKET _cSock)
{
	// �ֽڻ�����
	char szRecv[1024] = {};

	// 5 ���տͻ�������
	int nLen = recv(_cSock, szRecv, sizeof(DataHeader), 0);
	DataHeader* header = (DataHeader*)szRecv;
	if (nLen <= 0)
	{
		printf("�ͻ������˳������������\n");
		return -1;
	}
	switch (header->cmd) // ��Ϣͷ
	{
	case CMD_LOGIN:
	{

		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		Login* login = (Login*)szRecv; // �ṹ������
		printf("�յ����CMD_LOGIN ���ݳ��ȣ�%d,username=%s password=%s \n", login->dataLength, login->userName, login->passWord);
		LoginResult ret;
		send(_cSock, (char*)&ret, sizeof(LoginResult), 0); // ����Ϣ��
	}
	break;
	case CMD_LOGOUT:
	{

		recv(_cSock, szRecv + sizeof(DataHeader), header->dataLength - sizeof(DataHeader), 0);
		Logout* logout = (Logout*)szRecv; // �ṹ������
		printf("�յ����CMD_LOGOUT ���ݳ��ȣ�%d,username=%s \n", logout->dataLength, logout->userName);
		LogoutResult ret;
		send(_cSock, (char*)&ret, sizeof(LogoutResult), 0); // ����Ϣ��
	}
	break;
	default:
	{
		DataHeader header = { 0, CMD_ERROR };
		send(_cSock, (char*)&header, sizeof(header), 0); // �ȷ���Ϣͷ
	}
	break;
	}
}

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

	//char _recvBuf[128] = {};
	while (true) // ѭ���ȴ�
	{
		// socket����(������)
		fd_set fdRead;
		fd_set fdWrite;
		fd_set fdExp;

		FD_ZERO(&fdRead); // �� count��0
		FD_ZERO(&fdWrite);
		FD_ZERO(&fdExp);

		FD_SET(_sock, &fdRead);
		FD_SET(_sock, &fdWrite);
		FD_SET(_sock, &fdExp);

		for (int n = (int)g_clients.size() - 1; n >= 0; n--)
		{
			FD_SET(g_clients[n], &fdRead);
		}

		// nfds��һ������ֵ����ָfd_set����������������(socket)�ķ�Χ������������
		// ���������ļ����������ֵ+1 windows�п���д0
		timeval t = { 0,0 }; // ʱ����� ��ѯһ�����û�������
		int ret = select(_sock + 1, &fdRead, &fdWrite, &fdExp, &t); // ������������ģ��
		if (ret < 0)
		{
			printf("select���������\n");
			break;
		}
		if (FD_ISSET(_sock, &fdRead)) // socket�Ƿ�ɶ�
		{
			FD_CLR(_sock, &fdRead);
			// 4 accept �ȴ����տͻ�������
			sockaddr_in clientAddr;
			int nAddrLen = sizeof(sockaddr_in);
			SOCKET _cSock = INVALID_SOCKET;
			_cSock = accept(_sock, (sockaddr*)&clientAddr, &nAddrLen);
			if (INVALID_SOCKET == _cSock)
			{
				printf("���󣬽��յ���Ч�ͻ���SOCKET\n");
			}
			g_clients.push_back(_cSock);
			printf("�¿ͻ��˼��룺socket = %d IP = %s \n", (int)_cSock, inet_ntoa(clientAddr.sin_addr));

		}
		for (size_t n = 0; n < fdRead.fd_count; n++)
		{
			if (-1 == processor(fdRead.fd_array[n]))
			{
				auto iter = find(g_clients.begin(), g_clients.end(), fdRead.fd_array[n]);
				if (iter != g_clients.end())
				{
					g_clients.erase(iter);
				}
			}
		}



	}

	// �����׽���
	for (size_t n = g_clients.size() - 1; n >= 0; n--)
	{
		closesocket(g_clients[n]);
	}

	// 8 �ر��׽���closeSocket
	closesocket(_sock);

	// ���socket����
	WSACleanup();
	printf("���˳�,�������");
	getchar();
	return 0;
}