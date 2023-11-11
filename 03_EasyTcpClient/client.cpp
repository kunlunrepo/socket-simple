#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS // ���inet_ntoa

#include<windows.h>
#include<WinSock2.h>
#include<stdio.h>

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
		else
		{
			// 5 �������������������
			send(_sock, cmdBuf, strlen(cmdBuf) + 1, 0);
		}
		// 6 ���շ�������Ϣ recv
		char recvBuf[256] = {};
		int nlen = recv(_sock, recvBuf, 256, 0);
		if (nlen > 0)
		{
			printf("���յ����ݣ�%s\n", recvBuf);
		}
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