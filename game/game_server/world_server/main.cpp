#include "game_Server/world_server/world_server.h"
using namespace std;
using namespace zq;

int main(int argc, char* argv[])
{
	WorldServer server(argc, argv);
	server.start();
	server.run();

	return 0;
}




