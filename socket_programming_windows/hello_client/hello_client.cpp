// hello_client.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
void ErrorHandling(std::string msg);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char msg[30];
	int strLen;
	if (argc != 3)
	{
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	strLen = recv(hSocket, msg, sizeof(msg) - 1, 0);
	if (strLen == -1)
		ErrorHandling("read() error!");
	printf("Message from server: %s \n", msg);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(std::string msg)
{
	fputs(msg.c_str(), stderr);
	fputs("\n", stderr);
	exit(1);
}