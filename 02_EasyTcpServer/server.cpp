#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���inet_ntoa

#include<windows.h>
#include<WinSock2.h>
#include<stdio.h>

// ���崫�����ݰ�
struct DataPackage
{
	int age;
	char name[32];
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

	char _recvBuf[128] = {};
	while (true) // ѭ���ȴ�
	{
		// 5 ���տͻ�������
		int nLen = recv(_cSock, _recvBuf, 128, 0);
		if (nLen <= 0)
		{
			printf("�ͻ������˳������������\n");
			break;
		}
		printf("�յ����%s \n", _recvBuf);
		// 6 ��������
		if (0 == strcmp(_recvBuf, "getInfo"))
		{
			// 7 send ��ͻ��˷���һ������
			DataPackage dp = {80, "�Ź���"};
			send(_cSock, (const char*)&dp, sizeof(DataPackage), 0);

		}
		else if (0 == strcmp(_recvBuf, "getAge"))
		{
			// 7 send ��ͻ��˷���һ������
			char msgBuf[] = "80.";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}
		else
		{
			// 7 send ��ͻ��˷���һ������
			char msgBuf[] = "???.";
			send(_cSock, msgBuf, strlen(msgBuf) + 1, 0);
		}
	}
	// 8 �ر��׽���closeSocket
	closesocket(_sock);

	// ���socket����
	WSACleanup();
	printf("���˳�,�������");
	getchar();
	return 0;
}