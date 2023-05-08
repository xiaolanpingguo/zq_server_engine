#include "game_server/zone_server/zone_server.h"
using namespace std;
using namespace zq;

int main(int argc, char* argv[])
{
	ZoneServer server(argc, argv);
	if (!server.start())
	{
		printf("start server failed!\n");
		return -1;
	}

	server.run();
	return 0;
}




