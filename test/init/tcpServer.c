#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PORTNUM 9000
//#define BUFFER_SIZE 32

int system(const char* string);
int main(void){
	char buf[BUFSIZ] = {0,};
	struct sockaddr_in sin, cli;
	FILE *fp;
//	int n;
	int access_socket, comm_socket;
	int clientlen = sizeof(cli);
	int serverlen = sizeof(sin);

	printf("creating a socket...\n");
	if((access_socket = socket(AF_INET,SOCK_STREAM,0)) ==-1){
		perror("socket");
		exit(EXIT_FAILURE);
	}

	printf("building address..\n");
	memset((char *)&sin, '\0',serverlen);
	sin.sin_family =AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");

	printf("bind...\n");
	if(bind(access_socket,(struct sockaddr *)&sin, serverlen)){
		perror("bind");
		exit(EXIT_FAILURE);
	}
	printf("listen...\n");
	if(listen(access_socket, 5)){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	printf("accept\n");
	if((comm_socket =accept(access_socket, (struct sockaddr *)&cli,&clientlen)) ==-1){
		perror("accept");
		exit(EXIT_FAILURE);
	}
	printf("client is now accepted\n");

	while(1){
		memset(buf, '\0',BUFSIZ);
		fgets(buf, BUFSIZ, stdin);
		
	//	n = read(comm_socket,buf,BUFFER_SIZE);
//		printf("Recieved file name : %s\n",buf);
//		if(fp == NULL)
//			printf("file open failed!\n");
//		else
//			printf("File successfully opened!\n");
//		size_t num_read = fread(buf, 1, BUFFER_SIZE, fp);
		
//		n = write(comm_socket, buf, num_read);

		if(!strcmp(buf, "s\n")){
			system("arecord -d 10 /home/jisong/Desktop/test.mp3");
			sleep(6); //6sec
	//		fp = fopen(buf, "rb");
		}
		printf("USER INPUT: %s\n", "Sound recording");

		send(comm_socket, buf, strlen(buf)+1, 0);
		memset(buf, '\0', BUFSIZ);
		recv(comm_socket, buf, sizeof(buf), 0);
		if(recv(comm_socket, buf, sizeof(buf), 0) != -1){
			system("aplay /home/jisong/Desktop/test.mp3");
			printf("YOU: %s\n", "Sound recording");
		}
	}

//	if(fp != NULL)
//		fclose(fp);
	close(access_socket);
	close(comm_socket);

	return 0;
}
