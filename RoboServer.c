#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define PORT 8888
#define TIMEOUT 10

static void error(const char *msg);
static int socket_setup();
static void write_byte(int sockfd, char ch);
static int read_byte(int sockfd);

////

static int sockfd;
static unsigned long last;
static char values[16];

void remote_setup() {
	last = systime() - TIMEOUT;
	sockfd = socket_setup();
}

void remote_write(char ch) {
	write_byte(sockfd, ch);
}

int remote_read() {
	return read_byte(sockfd);
}

static void remote_update() {
	long new = systime();
	if(new - last < TIMEOUT)
		return;
	last = new;
	
	remote_write(0);
	int i;
	for(i = 0; i < 16; i++) {
		values[i] = remote_read();
	}
}

int remote_analog(int port) {
	remote_update();
	return (values[port] & 0xFF) << 2;
}

int remote_digital(int port) {
	remote_update();
	return values[port];
}

////

static void error(const char *msg) {
    perror(msg);
    exit(0);
}

static int socket_setup() {
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("ERROR opening socket");
	
	struct sockaddr_in serv_addr;
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(PORT);
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
		error("ERROR on binding");
	
	listen(sockfd, 5);
	
	struct sockaddr_in cli_addr;
	socklen_t clilen = sizeof(cli_addr);
	int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
	if (newsockfd < 0)
		error("ERROR on accept");
	
    close(sockfd);
	return newsockfd;
}

static void write_byte(int sockfd, char ch) {
    int n = write(sockfd, &ch, 1);
    if (n < 0) 
         error("ERROR writing to socket");
}

static int read_byte(int sockfd) {
	int received_int = 0;
	int uselessBuffer = read(sockfd, &received_int, sizeof(received_int));
	//printf("\n%d", received_int);
	return received_int;
}
