#include"server.h"

void thread_func(Server& server, std::vector<pac>& vec);

void Server::Init() {
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	if (WSAStartup(sockVersion, &wsaData) != 0)
	{
		return;
	}
}

void Server::Bind() {
	slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET)
	{

		printf("socket bind error ! \n");
		std::cout << "error code is " << GetLastError() << std::endl;
		return;
	}

	sockaddr_in sin;
	sin.sin_family = AF_INET; //ipv4 
	sin.sin_port = htons(8888); // port
	sin.sin_addr.S_un.S_addr = INADDR_ANY; // all ips welcomed
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind error ! \n");
		std::cout << "error code is " << GetLastError() << std::endl;
	}
}

void Server::Listen() {
	if (listen(slisten, 5) == SOCKET_ERROR) {

		printf("listen error ! \n");
		std::cout << "error code is " << GetLastError() << std::endl;
		return;
	}
}

bool Server::Process() {
	SOCKET client;
	sockaddr_in remote_addr;
	int addr_len = sizeof(remote_addr);

	std::cout << "waiting for connection... \n";
	client = accept(slisten, (SOCKADDR*)&remote_addr, &addr_len);
	
	if (client == INVALID_SOCKET) {
		std::cout << "accpet error \n ";
		std::cout << "error code is " << GetLastError() << std::endl;
		return false;
	}
		
	pac temp(client, remote_addr, sock_num);
	sock_vec.push_back(temp);

	std::cout << "connection established, ip is " << inet_ntoa(remote_addr.sin_addr) << '\n';
	sock_num++;
	std::thread th(thread_func, *this, sock_vec);
	th.detach();
	return true;
}

void thread_func(Server& server, std::vector<pac>& sock_pac) {
	while (1) {
		if (server.sub_process(sock_pac[server.sock_num]) == true) {
			break;
		}
	}
	closesocket(sock_pac[server.sock_num]._client);
}

bool Server::sub_process(pac& sock_pac) {
	std::string result;
	int times = 0;
	std::vector<char> buffer(MAX_BUF_LENGTH);
	int bit = 0;
	do {
		std::string cmdline;
		getline(std::cin, cmdline);
		send(sock_pac._client, cmdline.c_str(), cmdline.length(), 0);
		bit = recv(sock_pac._client, buffer.data(), buffer.size(), 0);
		if (bit == -1) {
			std::cerr << "error, client exit! \n";
			return true;
		}
		else {
			if (bit == 0) {
				continue;
			}
			if (bit == MAX_BUF_LENGTH) {
				result.append(buffer.data());
				times++;
				result.resize(MAX_BUF_LENGTH * times);
			}
			else {
				buffer[bit] = '\0';
				result += buffer.data();
			}
			if (result == "quit") {
				printf("[Socket %d exits ] \n", sock_pac._num + 1);
				char* exit_msg = "exit_success";
				send(sock_pac._client, exit_msg, strlen(exit_msg), 0);
				return true;
			}
			printf("[Socket %d ] > %s \n", sock_pac._num+1, result.c_str());
			send(sock_pac._client, sendData, strlen(sendData), 0);
			times = 0;
			result.clear();

		}

	} while (bit == MAX_BUF_LENGTH);
	return false;
}


