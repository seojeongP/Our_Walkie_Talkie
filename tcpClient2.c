#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>

#define PORTNUM 9000

int system(const char* string);
int main(void){
	char buf[BUFSIZ] = {0,};
	struct sockaddr_in sin;
	int sock;
	int serverlen = sizeof(sin);
	FILE *sendf, *recvf;
	char *c;
	int i;

	printf("creating a socket...\n");
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) ==-1){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	printf("building address..\n");
	memset((char *)&sin, '\0', serverlen);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("192.168.0.6");
	
	if(connect(sock,(struct sockaddr *)&sin, serverlen)){
		perror("connect");
		exit(EXIT_FAILURE);
	}
	printf("Now, connected to the server\n");

	while(1){
		memset(buf, '\0', BUFSIZ);
		if((recvf = fopen("/home/jisong/sp2021-2/pro/recv.mp3", "r+")) == NULL){
                        perror("fopen");
                        exit(1);
                }
		if(recv(sock, buf, sizeof(buf),0) != -1){
			if((i = fputs(buf, recvf))==-1)
			{
				perror("recv");
				exit(1);
			}
			//system("aplay /home/jisong/sp2021-2/pro/recv.mp3");
			//printf("YOU: %s\n", "Sound recording");
		}
		system("aplay /home/jisong/sp2021-2/pro/recv.mp3");
                printf("YOU: %s\n", "Sound recording");
		memset(buf, '\0', BUFSIZ);
		fgets(buf, BUFSIZ, stdin);
		
		printf("USER INPUT: %s\n","Sound recording");
		if(!strcmp(buf,"s\n"))
		{
			system("arecord -d 10 /home/jisong/sp2021-2/pro/send.mp3");
		}
		memset(buf, '\0', BUFSIZ);
		if((sendf = fopen("/home/jisong/sp2021-2/pro/send.mp3","r+")) == NULL){
			perror("fopen");
			exit(1);
		}
		if((c = fgets(buf, BUFSIZ,sendf))==NULL){
			perror("fgets");
			exit(1);	
		}
		send(sock,buf,strlen(buf)+1,0);
	}
	close(sock);
	return 0;
}
