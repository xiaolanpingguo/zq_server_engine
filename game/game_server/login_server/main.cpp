#include "game_server/login_server/login_server.h"
using namespace std;
using namespace zq;

int main(int argc, char* argv[])
{
	LoginServer server(argc, argv);
	if (!server.start())
	{
		printf("start server failed!\n");
		return -1;
	}

	server.run();
	return 0;
}




