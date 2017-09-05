#include"server.h"

void thread_func(Server server, SOCKET client);

struct sockpara {
	SOCKET _listen;
	SOCKET _client;
	sockaddr_in _remote_addr;
};

Server::~Server() {
	closesocket(slisten);
	WSACleanup();
}

void Server::Listen() {
	if (listen(slisten, 5) == SOCKET_ERROR) {
		printf("listen error !");
		return;
	}
}

void Server::Bind() {
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

void Server::Init() {
	//≥ı ºªØWSA  
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return;
	}
}

void Server::Process() {
	SOCKET client;
	sockaddr_in remote_addr;
	int addr_len = sizeof(remote_addr);

	std::cout << "waiting for connection... \n";

		client = accept(slisten, (SOCKADDR*)&remote_addr, &addr_len);
		if (client == INVALID_SOCKET) {
			std::cout << "accpet error \n ";
			return;
		}
		//accept a socket for connection	
		std::cout << "connection established, ip is " << inet_ntoa(remote_addr.sin_addr) << '\n';

		std::thread th(thread_func, *this, client);
		th.detach();
}

void thread_func(Server server, SOCKET client) {
	while (1) {
		if (server.sub_process(client) == true) {
			closesocket(client);
			break;
		}
	}
}

bool Server::sub_process(SOCKET client) {
	std::string result;
	int times = 0;
	std::vector<char> buffer(MAX_BUF_LENGTH);
	int bit = 0;
	do {
		bit = recv(client, buffer.data(), buffer.size(), 0);
		// aaaaaaaaaaaaaaaaaaaa aaaaaaaaaaaaaaaaaaaa$ cant solve
		if (bit == -1) {
			std::cerr << "error, client exit!";
			return true;
		}
		else {
			if (bit == 0) {
				continue;
			}
			if (bit == MAX_BUF_LENGTH) {
				result.append(buffer.data());// += buffer.data();
				times++;
				result.resize(MAX_BUF_LENGTH * times);
			}
			else {
				buffer[bit] = '\0';
				result += buffer.data();
			}
			if (result == "quit") {
				return true;
			}
			std::cout << result << std::endl;
			send(client, sendData, strlen(sendData), 0);
			times = 0;
			result.clear();

		}

	} while (bit == MAX_BUF_LENGTH);
	return false;
}


