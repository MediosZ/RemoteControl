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


struct pac {
	SOCKET _client;
	sockaddr_in _addr;
	int _num;

	pac(SOCKET cli, sockaddr_in so, int num) {
		_client = cli;
		_addr = so;
		_num = num;
	}
};


class Server {
public:
	SOCKET slisten;
	std::vector<pac> sock_vec;
	std::vector<pac>::iterator cu_it;
	int sock_num;

	Server():sock_num(-1) {}
	~Server();

	void Listen();
	void Bind();
	void Init();
	void Process();
	bool sub_process(pac& sock_pac);
};


