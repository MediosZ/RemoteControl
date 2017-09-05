#include"server.h"

int main()
{

	Server server;
	server.Init();
	server.Bind();
	server.Listen();
	while (1) {
		server.Process();
	}

	return 0;
}