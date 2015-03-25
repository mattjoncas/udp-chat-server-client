#include "Server.h"
#include "Caesar.h"

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa){
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

    	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

Server::Server(){
	if(!Init()){
		printf("Error initializing server.\n");
	}
}
Server::~Server(){
	
}

bool Server::Init(){
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return false;
	}
	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
			perror("socket\n");
			continue;
		}
		fcntl(sockfd, F_SETFL, O_NONBLOCK);
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("bind\n");
			continue;
		}
		break;
	}
	if (p == NULL) {
		fprintf(stderr, "failed to bind socket\n");
		return false;
	}
	freeaddrinfo(servinfo);
	addr_len = sizeof(their_addr); //THIS FIXED THE clients.begin() + 1 BUG!!
	printf("Server running. now we wait...\n");
	return true;
}
bool Server::Update(){
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0, (struct sockaddr *)&their_addr, &addr_len)) == -1) {
		return true;
	}
	//get identifier
	char c = buf[0];
	for (int i = 0; i < numbytes; i++){
		buf[i] = buf[i + 1];
	}
	buf[numbytes - 1] = '\0';
	//handle packet
	if (c == '0'){
		//add client to list
		printf("Client joined.\n");
		clients.push_back(*(struct sockaddr*)&their_addr);
		printf("clients size: %i\n", int(clients.size()));
	}
	else if (c == '1'){
		strcpy(buf, Caesar::Decrypt(std::string(buf)).c_str());
		//send message to all other clients
		for (int i = 0; i < clients.size(); i++){ //PRETTY SURE BOTH CLIENTS HAVE THE SAME IP ***ERROR***
			if(!CompareClients(&clients[i], (struct sockaddr *)&their_addr)){
				addr_len = sizeof(clients[i]);
				sendto(sockfd, buf, strlen(buf), 0, &clients[i], addr_len);
			}
		}
		printf("new message: \"%s\"\n", buf);
	}
	//remove client from list
	else if (c == '2'){
		printf("Client left.\n");
		for (int i = 0; i < clients.size(); i++){
			if(CompareClients(&clients[i], (struct sockaddr *)&their_addr)){					
				clients.erase(clients.begin() + 1);
				break;
			}
		}
		printf("clients size: %i\n", int(clients.size()));
		//shut down server if everyone leaves
		if (clients.size() == 0){
			return false;			
		}
	}
	//shut down server
	else if (c == '3'){
		return false;
	}
	return true;
}
void Server::Shutdown(){
	close(sockfd);
}
bool Server::CompareClients(struct sockaddr* c_1, struct sockaddr* c_2){
	if(inet_ntop(their_addr.ss_family, get_in_addr(c_1), s, sizeof s) == 
	inet_ntop(their_addr.ss_family, get_in_addr(c_2), s, sizeof s)){
		return true;
	}
	return false;
}
