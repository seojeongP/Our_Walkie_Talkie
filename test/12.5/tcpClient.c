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
	char buf[BUF_SIZ];
	char t[] = "true";
	char f[] = "false";
	struct sockaddr_in sin;
	int sock;
	int serverlen = sizeof(sin);
	FILE *sendf, *recvf;
	int c;
	int i, ab;
	char s_buf[5];

	printf("creating a socket...\n");
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) ==-1){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	printf("building address..\n");
	memset((char *)&sin, '\0', serverlen);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("192.168.200.133");
	
	if(connect(sock,(struct sockaddr *)&sin, serverlen)){
		perror("connect");
		exit(EXIT_FAILURE);
	}
	printf("Now, connected to the server\n");

	while(1){
		memset(s_buf, '\0', sizeof(s_buf));
		memset(buf, '\0', BUF_SIZ);
		printf("1\n");
		printf("buf size : %ld\n", sizeof(buf));
		recv(sock, s_buf,sizeof(buf),0);
		printf("s_buf : %s\n",s_buf);
		if(!strcmp(s_buf, "true")){
			if((ab = recv(sock, buf, sizeof(buf),0)) != -1){
			printf("2\n");
			printf("ab : %d\n", ab);
			if((recvf = fopen("/home/jisong/sp2021-2/pro/recv.mp3", "wb+")) == NULL){
                        	perror("fopen");
                        	exit(1);
                	}
			printf("3\n");
			if((i = fwrite(buf, BUF_SIZ, 1, recvf))<=0)
			{
				perror("recv");
				exit(1);
			}
			printf("4");
			}
		}
		
		printf("5\n");
		//system("aplay /home/jisong/sp2021-2/pro/recv.mp3");
		printf("6\n");
		printf("YOU: %s\n", "Sound recording");
		printf("7\n");
		system("aplay /home/jisong/sp2021-2/pro/recv.mp3");
		memset(s_buf, '\0', sizeof(s_buf));
		memset(buf, '\0', BUF_SIZ);	
		printf("8\n");
		fgets(s_buf, BUF_SIZ, stdin);
		printf("9\n");
		if(!strcmp(s_buf,"s\n"))
		{
			strcpy(s_buf,t);
			system("arecord -d 5 /home/jisong/sp2021-2/pro/send.mp3");
			printf("10\n");
		}
		printf("USER INPUT: %s\n","Sound recording");
		printf("11\n");
		if((sendf = fopen("/home/jisong/sp2021-2/pro/send.mp3","rb+")) == NULL){
			perror("fopen");
			exit(1);
		}
		printf("12\n");
		if((c = fread(buf, BUF_SIZ, 1, sendf))<=0){
			perror("fgets");
			exit(1);	
		}
		printf("13\n");
		send(sock,s_buf,sizeof(s_buf),0);
		send(sock,buf,sizeof(buf),0);
		printf("14\n");
	}
	fclose(sendf);
	fclose(recvf);
	close(sock);
	return 0;
}
