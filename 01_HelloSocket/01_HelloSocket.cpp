#define WIN32_LEAN_AND_MEAN // 避免引入早起冲突库的引用

#include<windows.h>
#include<WinSock2.h>

// #pragma comment(lib, "ws2_32.lib") // 引入动态链接库

int main()
{
	// 启动windows socket 2.x环境
	WORD ver = MAKEWORD(2,2); // 版本
	WSADATA dat;
	// 启动socket函数
	WSAStartup(ver, &dat);

	WSACleanup();
	return 0;
}