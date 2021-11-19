#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>

//#define BUFFER_SIZE 32
#define PORTNUM 9000

int system(const char* string);
int main(void){
	char buf[BUFSIZ] = {0,};
	struct sockaddr_in sin;
	int sock;
//	int n;
	int serverlen = sizeof(sin);
	FILE *out;

	printf("creating a socket...\n");
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) ==-1){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	printf("building address..\n");
	memset((char *)&sin, '\0', serverlen);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	if(connect(sock,(struct sockaddr *)&sin, serverlen)){
		perror("connect");
		exit(EXIT_FAILURE);
	}
	printf("Now, connected to the server\n");

	while(1){
		memset(buf, '\0', BUFSIZ);
		recv(sock, buf, sizeof(buf),0);
		if(recv(sock, buf, sizeof(buf),0) != -1){
			system("aplay /home/jisong/Desktop/test.mp3");
			printf("YOU: %s\n", "Sound recording");
		}
//		n = write(sock, buf, strlen(buf));

		if(!strcmp(buf, "s\n")){
                        system("arecord -d 5 /home/jisong/Desktop/test.mp3");
                       //	out = fopen("arecord -d 5 home/jisong/Desktop/test.mp3", "wb");
		       	sleep(6); //6sec
                }
		
//		n = read(sock, buf, BUFFER_SIZE);

		memset(buf, '\0', BUFSIZ);
		fgets(buf, BUFSIZ, stdin);

		printf("USER INPUT: %s\n","Sound recording");
		send(sock,buf,strlen(buf)+1,0);
//		fwrite(buf, 1, n, out);
	}
//	if(out != NULL)
//		fclose(out);
	close(sock);
	return 0;
}
