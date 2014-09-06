#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <signal.h>

#define BUFFER_SIZE 256

//http://courses.cs.washington.edu/courses/cse476/02wi/labs/lab1/
void error(const char *msg) {
	perror(msg);
	exit(0);
}

void cleanexit() {
	exit(0);
}

int main(int argc, char *argv[]) {
	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	char buffer[BUFFER_SIZE];
	if (argc != 3) {
		fprintf(stderr, "usage %s <hostname> <port>\n", argv[0]);
		exit(0);
	}

	signal(SIGTERM, cleanexit);
	signal(SIGINT, cleanexit);

	// First we need to create a socket
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");

	// Next we get the address information of the server
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr,
			server->h_length);
	serv_addr.sin_port = htons(portno);

	// We establish the connection
	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERROR connecting");

	// Now we communicate with the server 
	printf("Please enter the message: ");
	bzero(buffer, BUFFER_SIZE);
	fgets(buffer, BUFFER_SIZE - 1, stdin);
	n = write(sockfd, buffer, strlen(buffer));
	if (n < 0)
		error("ERROR writing to socket");

	bzero(buffer, BUFFER_SIZE);
	n = read(sockfd, buffer, BUFFER_SIZE - 1);
	if (n < 0)
		error("ERROR reading from socket");
	printf("Server says: %s\n", buffer);

	// Finally we close the connection
	close(sockfd);

	return 0;
}
