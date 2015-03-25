#include "Server.h"

Server server;

int main(void){
	bool running = true;
	while (running){
		running = server.Update();
	}
	server.Shutdown();
	return 0;
}
