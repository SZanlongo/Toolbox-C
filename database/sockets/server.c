#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <pthread.h>

#define BUFFER_SIZE 256

void error(const char *msg) {
	perror(msg);
	exit(0);
}

void cleanexit() {
	exit(0);
}

void *server_response(void *sockfd_ptr) {
	char buffer[BUFFER_SIZE];
	int n;
	int newsockfd = (int) sockfd_ptr;

	// Get message from client and send response 
	bzero(buffer, BUFFER_SIZE);
	n = read(newsockfd, buffer, BUFFER_SIZE - 1);
	if (n < 0)
		error("ERROR reading from socket");
	printf("Here is the message: %s\n", buffer);

	n = write(newsockfd, "I got your message", 18);
	if (n < 0)
		error("ERROR writing to socket");

	// Close connection to client
	close(newsockfd);

	return 0;
}

int main(int argc, char *argv[]) {
	int sockfd, newsockfd, portno;
	socklen_t clilen;
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	if (argc != 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}

	signal(SIGTERM, cleanexit);
	signal(SIGINT, cleanexit);

	// Create socket to listen for connections
	portno = atoi(argv[1]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	// Bind port to socket
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR on binding");

	// Listen for incoming connections
	listen(sockfd, 5);
	clilen = sizeof(cli_addr);

	while (1) {
		// Accept a client connection
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0)
			error("ERROR on accept");

		printf("Connection from: %s\n", inet_ntoa(cli_addr.sin_addr));

		pthread_t tid;
		pthread_create(&tid, NULL, server_response, (void *) newsockfd);
	}

	// Close listening socket
	close(sockfd);

	return 0;
}
