#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 32
#define PORTNUM 9000

int system(const char *string);

void error(const char *msg){
	perror(msg);
	exit(1);
}

int main(){
	int sd, ns, n;
	socklen_t clilen, servlen;

	char buffer[BUFFER_SIZE], buf[2];
	FILE *fp;
	struct sockaddr_in serv_addr, cli_addr;

	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd<0)
		error("ERROR opening socket");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(PORTNUM);

	if(bind(sd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		error("ERRROR on binding");

	listen(sd, 5);

	clilen = sizeof(cli_addr);
	servlen = sizeof(serv_addr);
	ns = accept(sd, (struct sockaddr *) &cli_addr, &clilen);
	if(ns < 0)
		error("ERROR on accept");

	printf("Enter key : ");
	scanf(buf,"%s");

	if(strcmp(buf, "s")){
		system("arecord -d 5 /home/seojeong/Desktop/systemprogramming/test/test.mp3");
		sleep(6);
	}
	printf("USER INPUT: %s\n", "Sound recording");


	fp = fopen("test.mp3", "rb");
	if(fp == NULL)
		printf("file open failed!\n");
	else
		printf("File successfully opened!\n");

	while(1){
		size_t num_read = fread(buffer, 1, BUFFER_SIZE, fp);
		if(num_read == 0)
			break;

		n = write(ns, buffer, num_read);
		if(n < 0)
			error("Error writing to socket");
		else if(n==0)
			break;

	}

	printf("File sending complete..\n");

	if(fp != NULL)
		fclose(fp);
	close(ns);
	close(sd);

	return 0;
}
