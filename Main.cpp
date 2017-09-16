#include"server.h"

int main()
{
	Server server;
	server.Init();
	server.Bind();

	while (1) {
		server.Listen();
		if (server.Process() == false) {
			break;
		}		
	}

	closesocket(server.slisten);
	WSACleanup();
	return 0;
}