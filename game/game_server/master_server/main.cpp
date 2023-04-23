#include "game_server/master_server/master_server.h"
using namespace std;
using namespace zq;

int main(int argc, char* argv[])
{
	MasterServer server(argc, argv);
	server.start();
	server.run();

	return 0;
}




