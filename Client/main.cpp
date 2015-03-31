#include "Client.h"

Client client;

void c_send(){
	client.Send();
}
void c_recv(){
	client.Recv();
}

int main(int argc, char *argv[]){
	
	if (argc != 2) {
		fprintf(stderr,"enter hostname\n");
		return 0;
	}

	if(!client.Init(argv[1])){
		printf("Error initializing client.\n");
		return 0;
	}
	
	std::thread r_thread = std::thread(c_recv);
	std::thread s_thread = std::thread(c_send);
	r_thread.join();
	s_thread.join();

    return 0;
}
