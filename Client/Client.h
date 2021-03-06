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
#include <thread>

#define SERVERPORT "4950"

#define MAXBUFLEN 100

class Client{
public:
	Client();
	~Client();

	bool Init(const char* host);
	void Send();
	void Recv();
private:
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;

	char buf[MAXBUFLEN];

	bool running;
};
