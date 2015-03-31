#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <iostream>
#include <vector>

#define MYPORT "4950"

#define MAXBUFLEN 100

class Server {
public:
	Server();
	~Server();

	bool Update();
	void Shutdown();
private:
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	struct sockaddr_storage their_addr;
	std::vector<struct sockaddr> clients;
	char buf[MAXBUFLEN];
	socklen_t addr_len;
	char s[INET6_ADDRSTRLEN];

	bool Init();
	bool CompareClients(struct sockaddr* c_1, struct sockaddr* c_2);
};
