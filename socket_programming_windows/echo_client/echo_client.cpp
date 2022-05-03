// echo_client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

void Error_Handling(std::string msg);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSock;
	SOCKADDR_IN servAddr;
	char msg[1024];
	int strLen;

	if (argc != 3)
	{
		printf("Usage: %s <ip> <port> \n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		Error_Handling("WSAStartup() error.");

	hSock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		Error_Handling("connect() error.");
	else
		printf("Connected......");

	while (1)
	{
		fputs("Input message(Q to quit): \n", stdout);
		fgets(msg, 1024, stdin);

		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
			break;

		send(hSock, msg, strlen(msg), 0);
		strLen = recv(hSock, msg, 1023, 0);
		msg[1023] = 0;
		printf("Message from server: %s\n", msg);
	}
	closesocket(hSock);
	WSACleanup();
	return 0;
}

void Error_Handling(std::string msg)
{
	printf("%s \n", msg.c_str());
	exit(1);
}
// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
