// op_client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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
	SOCKET sockClnt;
	SOCKADDR_IN servAddr;
	char msg[BUF_SIZE];

	if (argc != 3)
	{
		printf("Usage: %s <ip> <port> \n", argv[0]);
		exit(1);
	}

	WSAStartup(MAKEWORD(2, 2), &wsaData);

	//init socket
	sockClnt = socket(PF_INET, SOCK_STREAM, 0);
	if (sockClnt == INVALID_SOCKET)
		error_handling("socket() error.");

	printf("ip: %s, port: %s \n", argv[1], argv[2]);
	//set server addr
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	//connect
	if (connect(sockClnt, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		error_handling("connect() error.");

	//set content
	memset(msg, 0, BUF_SIZE);
	printf("please input the operate number count: \n");
	int op_cnt;
	scanf("%d", &op_cnt);
	if (op_cnt == 0)
		error_handling("the input is error!");

	int index = 0;
	*(int*)msg = op_cnt;
	index += 4;
	for (int i = 0; i < op_cnt; i++)
	{
		printf("please input the number: \n");
		int op_value = 0;
		scanf("%d", &op_value);
		*(int*)(msg + index) = op_value;
		index += 4;
	}
	printf("please input an operator: + - * / \n");
	getchar();
	char op_str;
	scanf("%c", &op_str);
	*(char*)(msg + index) = op_str;
	index += 1;
	*(char*)(msg + index) = 'e';
	index += 1;

	//send and recv
	if (send(sockClnt, msg, index, 0) == SOCKET_ERROR)
		error_handling("send() error.");
	int recv_len = 0;
	while ((recv_len = recv(sockClnt, msg, BUF_SIZE, 0)) <= 0)
		continue;
		
	int answer = *(int *)msg;
	printf("the answer is: %d\n", answer);

	closesocket(sockClnt);
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
