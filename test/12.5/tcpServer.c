#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PORTNUM 9001
#define BUF_SIZ 80044

int system(const char* string);
int main(void){
	char buf[BUF_SIZ];
	char t[] = "true";
	char f[] = "false";
	struct sockaddr_in sin, cli;
	FILE *sendf, *recvf;
	int i;
	int c;
	char s_buf[5];
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
	sin.sin_addr.s_addr = inet_addr("192.168.200.133");

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
		memset(s_buf, '\0', sizeof(s_buf));
		memset(buf, '\0',BUF_SIZ);
		fgets(s_buf, BUF_SIZ, stdin);
		printf("1\n");	

		if(!strcmp(s_buf, "s\n")){
			strcpy(s_buf,t);
			system("arecord -d 5 /home/jisong/sp2021-2/pro/send.mp3");
			printf("2\n");
		}
		printf("USER INPUT: %s\n", "Sound recording");
		printf("3\n");
		if((sendf = fopen("/home/jisong/sp2021-2/pro/send.mp3", "rb+")) == NULL){
			perror("fopen: send.mp3");
			exit(1);
		}
		printf("4\n");
		if((c = fread(buf,  BUF_SIZ,1, sendf)) <= 0){
			perror("get file");
			exit(1);
		}
		printf("5\n");
		printf("c : %d\n", c);
		printf("6\n");
		printf("strlen(buf) : %ld\n", strlen(buf)+1);
		printf("7\n");
		printf("sizeof(buf) : %ld\n", sizeof(buf));
		printf("8\n");
		int ab;
		send(comm_socket,s_buf,sizeof(s_buf),0);
		ab  = send(comm_socket, buf, sizeof(buf), 0);
		printf("ab : %d\n", ab);
		printf("9\n");
		memset(s_buf,'\0', sizeof(s_buf));
		memset(buf, '\0', BUF_SIZ);
		printf("10\n");
		recv(comm_socket, s_buf, sizeof(s_buf), 0);
		printf("11\n");
		printf("s_buf : %s\n", s_buf);
		if(!strcmp(s_buf,"true")){
			if(recv(comm_socket, buf, BUF_SIZ, 0) != -1){
			if((recvf = fopen("/home/jisong/sp2021-2/pro/recv.mp3", "wb+")) == NULL){
				perror("fopen: recv.mp3");
				exit(1);
			}
			printf("12\n");
			if((i = fwrite(buf, BUF_SIZ,1, recvf)) <= 0){
				perror("put file");
				exit(1);
			}
			printf("13\n");
			}
		}
		system("aplay /home/jisong/sp2021-2/pro/recv.mp3");
		printf("14\n");
                printf("YOU: %s\n", "Sound recording\n");
		printf("15\n");
	}
	fclose(sendf);
	fclose(recvf);
	close(access_socket);
	close(comm_socket);

	return 0;
}
