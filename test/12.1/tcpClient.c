#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORTNUM 9001
#define BUF_SIZ 80044
int system(const char* string);
int main(void){
	char buf[BUF_SIZ] = {0,};
	struct sockaddr_in sin;
	int sock;
	int serverlen = sizeof(sin);
	FILE *sendf, *recvf;
	int c;
	int i, ab;

	printf("creating a socket...\n");
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) ==-1){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	printf("building address..\n");
	memset((char *)&sin, '\0', serverlen);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("192.168.0.4");
	
	if(connect(sock,(struct sockaddr *)&sin, serverlen)){
		perror("connect");
		exit(EXIT_FAILURE);
	}
	printf("Now, connected to the server\n");

	while(1){
		memset(buf, '\0', BUF_SIZ);
		/*if((recvf = fopen("/home/jisong/sp2021-2/pro/recv.mp3", "wb+")) == NULL){
                        perror("fopen");
                        exit(1);
                }*/
		//recv(sock, buf, BUF_SIZ, 0);
		printf("buf size : %ld\n", sizeof(buf));
		if((ab = recv(sock, buf, BUF_SIZ,0)) != -1){
			printf("ab : %d\n", ab);
			if((recvf = fopen("/home/jisong/sp2021-2/pro/recv.mp3", "wb+")) == NULL){
                        	perror("fopen");
                        	exit(1);
                	}

			if((i = fwrite(buf, BUF_SIZ, 1, recvf))<=0)
			{
				perror("recv");
				exit(1);
			}
			printf("i : %d\n", i);
			
			//system("aplay /home/jisong/sp2021-2/pro/recv.mp3");
			//printf("YOU: %s\n", "Sound recording");
		}
		system("aplay /home/jisong/sp2021-2/pro/recv.mp3");
		
		printf("YOU: %s\n", "Sound recording");
		memset(buf, '\0', BUF_SIZ);
		fgets(buf, BUF_SIZ, stdin);
		
		printf("USER INPUT: %s\n","Sound recording");
		if(!strcmp(buf,"s\n"))
		{
			system("arecord -d 5 /home/jisong/sp2021-2/pro/send.mp3");
		}
		//memset(buf, '\0', BUF_SIZ);
		if((sendf = fopen("/home/jisong/sp2021-2/pro/send.mp3","rb+")) == NULL){
			perror("fopen");
			exit(1);
		}
		if((c = fread(buf, BUF_SIZ, 1, sendf))<=0){
			perror("fgets");
			exit(1);	
		}
		send(sock,buf,sizeof(buf),0);
	}
	close(sock);
	return 0;
}
