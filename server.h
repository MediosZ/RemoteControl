#include<stdio.h>
#include<winsock2.h>
#include<windows.h>
#include<string>
#include<vector>
#include<iostream>
#include<thread>

#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996)

#define MAX_BUF_LENGTH 256
#define sendData "hello client~ \n"

class Server {
public:
	SOCKET slisten;

	Server() {}
	~Server();

	void Listen();
	void Bind();
	void Init();
	void Process();
	bool sub_process(SOCKET client);
};
