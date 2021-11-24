#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define BUFFER_SIZE 32
#define PORT 9000

int system(const char *string);

void error(const char *msg){
	perror(msg);
	exit(1);
}

int main(){
	int sockfd, portno, n;
	struct hostent *server;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in serv_addr;
	FILE *out;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0)
		error("ERROR opening socket");

	server = gethostbyname("localhost");
	if(server == NULL){
		fprintf(stderr, "ERROR, no such host\n");
		exit(0);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(PORT);

	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERRROR connecting");

	out = fopen("out.mp3", "wb");
	while(1){
		n = read(sockfd, buffer, BUFFER_SIZE);
		if( n<0 )
			error("ERROR reading from socket");
		else if(n==0)
			break;
		
		fwrite(buffer, 1, n, out);

	}

	printf("File write complete... You can now use the output file!!\n");

	if( out != NULL)
		fclose(out);
	close(sockfd);

	return 0;
}
