// op_server.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
void error_handling(std::string msg)
{
	printf("%s", msg.c_str());
	exit(1);
}

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET sockServ, sockClnt;
	SOCKADDR_IN servAddr;
	char msg[BUF_SIZE];

	if (argc != 2)
	{
		printf("Usage: %s <port> \n", argv[0]);
		exit(1);
	}

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//init socket
	sockServ = socket(PF_INET, SOCK_STREAM, 0);
	if (sockServ == INVALID_SOCKET)
		error_handling("socket() error.");

	//set server addr
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	//bind
	if (bind(sockServ, (SOCKADDR *)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		error_handling("bind() error.");

	//listen
	if (listen(sockServ, 5) == SOCKET_ERROR)
		error_handling("listen() error.");

	//accept
	SOCKADDR_IN clntAddr;
	int clntAddrSize = sizeof(clntAddr);
	sockClnt = accept(sockServ, (SOCKADDR*)&clntAddr, &clntAddrSize);
	if (sockClnt == INVALID_SOCKET)
		error_handling("accept() error.");

	//receive and compute
	int recvLen = 0;
	while (recv(sockClnt, msg + recvLen, 1, 0) != 0)
	{
		if (msg[recvLen] != 'e')
		{
			recvLen += 1;
			continue;
		}
		recvLen += 1;
		break;
	}

	int index = 0;
	int op_count = *(int *)(msg + index);	index += 4;
	char op_str = *(char *)(msg + index + 4 * op_count);
	int op_result = 0;
	for (int i = 0; i < op_count; i++)
	{
		int op_val = *(int *)(msg + index);		index += 4;
		if (i == 0)
			op_result = op_val;
		else
		{
			if (op_str == '+')
				op_result += op_val;
			else if (op_str == '-')
				op_result -= op_val;
			else if (op_str == '*')
				op_result *= op_val;
			else if (op_str == '/')
				op_result /= op_val;
		}
	}
	memset(msg, 0, BUF_SIZE);
	index = 0;
	*(int *)(msg + index) = op_result;
	index += 4;
	*(char *)(msg + index) = 'e';
	index += 1;

	//send
	send(sockClnt, msg, index, 0);

	closesocket(sockClnt);
	closesocket(sockServ);
	WSACleanup();
	return 0;
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
