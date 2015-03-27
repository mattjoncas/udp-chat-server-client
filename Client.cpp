#include "Client.h"
#include "Caesar.h"

Client::Client(){

	running = true;
	
}
Client::~Client(){
	
}

bool Client::Init(const char* host){
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if ((rv = getaddrinfo(host, SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return false;
	}
	
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
    			perror("socket\n");
    			continue;
		}
		fcntl(sockfd, F_SETFL, O_NONBLOCK);
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "failed to bind socket\n");
		return false;
	}
	//send initial message to server
	std::string msg = "0New client added.";
	if ((numbytes = sendto(sockfd, msg.c_str(), strlen(msg.c_str()), 0, p->ai_addr, p->ai_addrlen)) == -1) {
       		perror("init sendto\n");
	 	return false;
 	}	
	
	return true;
}

void Client::Send(){
	std::string msg;
	
	while(running){
		getline(std::cin, msg);
		if (msg == "exit"){
			running = false;
			msg = "2";
			if ((numbytes = sendto(sockfd, msg.c_str(), strlen(msg.c_str()), 0, p->ai_addr, p->ai_addrlen)) == -1) {
				perror("sendto error\n");
		 	}
		}
		else{
			msg = Caesar::Encrypt(msg);
			msg = "1" + msg;
		 	if ((numbytes = sendto(sockfd, msg.c_str(), strlen(msg.c_str()), 0, p->ai_addr, p->ai_addrlen)) == -1) {
		       		perror("sendto error\n");
		 	}
		}
	}

    freeaddrinfo(servinfo);
    close(sockfd);
}

void Client::Recv(){
	while (running){
		//get all pending messages
		if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, p->ai_addr, &p->ai_addrlen)) != -1) { //NULL, NULL
			buf[numbytes] = '\0';
			printf("%s\n", buf);
		}
	}
}
