#include "Client.h"

Client client;

int main(int argc, char *argv[]){
	
	if (argc != 2) {
		fprintf(stderr,"enter hostname\n");
		exit(1);
	}

	if(!client.Init(argv[1])){
		printf("Error initializing client.\n");
		return 0;
	}
	client.Run();

    return 0;
}
