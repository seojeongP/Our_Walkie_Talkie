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
	char buf[BUF_SIZ] = {0,}, null_buf[5];
	struct sockaddr_in sin;
	int sock, f_len;
	int serverlen = sizeof(sin);
	FILE *sendf, *recvf;
	int c, sum;
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
	sin.sin_addr.s_addr = inet_addr("192.168.0.105");
	
	if(connect(sock,(struct sockaddr *)&sin, serverlen)){
		perror("connect");
		exit(EXIT_FAILURE);
	}
	printf("Now, connected to the server\n");
//	fclose(recvf);

	printf("open close\n");
	while(1){
		sum = 0;
		//receive file size
		recv(sock, buf, BUF_SIZ, 0);
		f_len = atoi(buf);
		printf("f_len : %d\n", f_len);
		sum = f_len;
		memset(buf, '\0', f_len);

		printf("please enter the any key\n");
		fgets(null_buf, sizeof(null_buf), stdin);

		if(access("/home/seojeongp/Desktop/my_test/recv.mp3", F_OK) == 0)
			remove("/home/seojeongp/Desktop/my_test/recv.mp3");
		printf("buf size : %ld\n", sizeof(buf));
		while(sum>0){
			printf("restart\n");
			printf("sum : %d\n", sum);
			if((ab = recv(sock, buf, sum-1,0)) == -1){
				perror("recv");
				exit(1);	
			}
			printf("ab : %d\n", ab);

                        if((recvf = fopen("/home/seojeongp/Desktop/my_test/recv.mp3", "wb+")) == NULL){
                                perror("fopen");
                                exit(1);
                        }

                        if((i = fwrite(buf, BUF_SIZ, 1, recvf))<=0)
                        {
                                perror("recv");
                                exit(1);
                        }
                        printf("i : %d\n", i);
			sum -= ab;
			printf("sum : %d\n", sum);

		}
		system("aplay /home/seojeongp/Desktop/my_test/recv.mp3");
		
		printf("YOU: %s\n", "Sound recording");
		memset(buf, '\0', BUF_SIZ);
		fgets(buf, BUF_SIZ, stdin);
		
		printf("USER INPUT: %s\n","Sound recording");
		if(!strcmp(buf,"s\n"))
		{
			system("arecord -d 5 /home/seojeongp/Desktop/my_test/test.mp3");
		}
		//memset(buf, '\0', BUF_SIZ);
		if((sendf = fopen("/home/seojeongp/Desktop/my_test/test.mp3","rb+")) == NULL){
			perror("fopen");
			exit(1);
		}
		if((c = fread(buf, BUF_SIZ, 1, sendf))<=0){
			perror("fgets");
			exit(1);	
		}
		send(sock,buf,sizeof(buf),0);
		printf("USER inpu Sound recoding\n");
	}
	close(sock);
	return 0;
}
