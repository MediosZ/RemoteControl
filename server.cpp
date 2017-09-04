#include<stdio.h>
#include<fstream>
#include<winsock2.h>
#include<windows.h>
#include<string>
#include<vector>
#include<iostream>
#include<thread>


#pragma comment(lib,"ws2_32.lib") 
#pragma warning(disable:4996)

#define MAX_BUF_LENGTH 256




class test {
public:
	SOCKET slisten;

	test() {}
	~test() {
		closesocket(slisten);
		WSACleanup();
	}

	void Listen() {
		if (listen(slisten, 5) == SOCKET_ERROR) {
			printf("listen error !");
			return;
		}
	}

	void Bind() {
		slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (slisten == INVALID_SOCKET)
		{
			printf("socket error !");
			return;
		}

		sockaddr_in sin;
		sin.sin_family = AF_INET;
		sin.sin_port = htons(8888);
		sin.sin_addr.S_un.S_addr = INADDR_ANY;
		if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
		{
			printf("bind error !");
		}
	}
	void Init() {
		//≥ı ºªØWSA  
		WORD sockVersion = MAKEWORD(2, 2);
		WSADATA wsaData;
		if (WSAStartup(sockVersion, &wsaData) != 0)
		{
			return;
		}
	}

	void Process();

	void MTprocess(SOCKET client, bool cnc_flag, bool end_flag, int times, sockaddr_in remote_addr, std::string result, const char* sendData) {
		if (cnc_flag) {
			std::cout << "connection established, ip is " << inet_ntoa(remote_addr.sin_addr) << '\n';
			cnc_flag = false;
		}
		std::vector<char> buffer(MAX_BUF_LENGTH);
		int bit = 0;
		do {
			bit = recv(client, buffer.data(), buffer.size(), 0);
			// aaaaaaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaaaaaa$ cant solve
			if (bit == -1) {
				std::cerr << "error ";
				return;
			}
			else {
				if (bit == 0) {
					continue;
				}
				if (buffer[bit - 1] == '$') {
					end_flag = false;
					buffer[bit - 1] = '\0';
				}
				if (bit == MAX_BUF_LENGTH) {
					result.append(buffer.data());// += buffer.data();
												 //result.erase(result.find_last_not_of(" ") + 1);
					times++;
					result.resize(MAX_BUF_LENGTH * times);
				}
				else {
					result.append(buffer.data());
					result.append(" ");
				}
				if (end_flag == false) {
					std::cout << result << std::endl;
					send(client, sendData, strlen(sendData), 0);
					end_flag = true;
					times = 0;
					result.clear();
				}
				//cout << buffer.data() << endl;

			}

		} while (bit == MAX_BUF_LENGTH);

		closesocket(client);
	}
};



void MTprocess(test& t) {
	std::thread sub_thread(std::bind(&test::Process, &t));
	sub_thread.detach();
}


int main()
{

	test t;
	t.Init();
	t.Bind();
	while (1) {
		t.Listen();//the server is waiting for socket, and accpet ome when needed
		//MTprocess(t);
		t.Process();
		//std::thread sub(std::bind(&test::Process, &t));
		//sub.detach();
	}

	//t.Connect();

	return 0;
}

void test::Process() {
	SOCKET client;
	sockaddr_in remote_addr;
	int addr_len = sizeof(remote_addr);
	std::string result;
	bool cnc_flag = true;
	bool end_flag = true;
	int times = 0;
	const char * sendData = "hello client£°\n";
	std::cout << "waiting for connection... \n";

	while (1) {
		client = accept(slisten, (SOCKADDR*)&remote_addr, &addr_len);
		if (client == INVALID_SOCKET) {
			std::cout << "accpet error \n ";
			continue;
		}
		//accept a socket for connection
		//MTprocess(client, cnc_flag, end_flag, times, remote_addr, result, sendData);
		if (cnc_flag) {
			std::cout << "connection established, ip is " << inet_ntoa(remote_addr.sin_addr) << '\n';
			cnc_flag = false;
		}
		std::vector<char> buffer(MAX_BUF_LENGTH);
		int bit = 0;
		do {
			bit = recv(client, buffer.data(), buffer.size(), 0);
			// aaaaaaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaaaaaa$ cant solve
			if (bit == -1) {
				std::cerr << "error ";
				return;
			}
			else {
				if (bit == 0) {
					continue;
				}
				if (buffer[bit - 1] == '$') {
					end_flag = false;
					buffer[bit - 1] = '\0';
				}
				if (bit == MAX_BUF_LENGTH) {
					result.append(buffer.data());// += buffer.data();
												 //result.erase(result.find_last_not_of(" ") + 1);
					times++;
					result.resize(MAX_BUF_LENGTH * times);
				}
				else {
					result.append(buffer.data());
					result.append(" ");
				}
				if (end_flag == false) {
					std::cout << result << std::endl;
					send(client, sendData, strlen(sendData), 0);
					end_flag = true;
					times = 0;
					result.clear();
				}
				//cout << buffer.data() << endl;

			}

		} while (bit == MAX_BUF_LENGTH);
		closesocket(client);
	}
}

