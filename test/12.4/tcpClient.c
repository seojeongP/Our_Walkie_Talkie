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
	char buf[BUF_SIZ] = {0,}, null_buf[5], filename[BUF_SIZ];
	struct sockaddr_in sin;
	int sock, f_len, songnum, in_f_len;
	int serverlen = sizeof(sin);
	FILE *sendf, *recvf;
	int c, sum;
	int i, ab;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) ==-1){
		perror("socket");
		exit(EXIT_FAILURE);
	}
	memset((char *)&sin, '\0', serverlen);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("192.168.0.105");
	
	if(connect(sock,(struct sockaddr *)&sin, serverlen)){
		perror("connect");
		exit(EXIT_FAILURE);
	}

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

		if((recvf = fopen("/home/seojeongp/Desktop/my_test/recv.mp3", "wb+")) == NULL){
                        perror("fopen");
                        exit(1);
                }

		while((ab = recv(sock, buf, sum, 0))>0){
			printf("sum : %d\n", sum);
			if(ab == -1){
				perror("recv");
				exit(1);	
			}

			printf("ab : %d\n", ab);
                        if((i = fwrite(buf, ab, 1, recvf))<=0)
                        {
                                perror("recv");
                                exit(1);
                        }

			sum -= ab;
			if(sum <= 0)
				break;

		}
		system("aplay /home/seojeongp/Desktop/my_test/recv.mp3");
		
		memset(buf, '\0', BUF_SIZ);
		printf("Enter s(record), m(music), q(quit) : ");
		fgets(buf, BUF_SIZ, stdin);
		
		if(!strcmp(buf,"s\n"))
		{
			printf("USER INPUT: %s\n","Sound recording");
			system("arecord -d 5 /home/seojeongp/Desktop/my_test/test.mp3");
			strcpy(filename, "/home/seojeongp/Desktop/my_test/test.mp3");
		}

		if(!strcmp(buf, "m\n"))
		{
			printf("choose song !\n");
			printf("1.sokodomo\n2.mudd the student\n3.aespa\n");
			printf("Enter the song number : ");
			scanf("%d", &songnum);

			switch(songnum){
				case 1:
					strcpy(filename, "/home/seojeongp/Desktop/my_test/merry.mp3");
					break;
				case 2:
					strcpy(filename, "/home/seojeongp/Desktop/my_test/mudd.mp3");
					break;
				case 3:
					strcpy(filename, "/home/seojeongp/Desktop/my_test/savage.mp3");
			}
		}

		if(!strcmp(buf, "q\n")){
			printf("Thank you for using program !! :-)\n");
			exit(1);
		}

		//memset(buf, '\0', BUF_SIZ);
		if((sendf = fopen(filename,"rb+")) == NULL){
			perror("fopen");
			exit(1);
		}

		fseek(sendf, 0, SEEK_END);
		in_f_len = ftell(sendf);
		fseek(sendf, 0, SEEK_SET);
		
		memset(buf, '\0', in_f_len);

		if((c = fread(buf, in_f_len, 1, sendf))<=0){
			perror("fgets");
			exit(1);	
		}
		send(sock,buf,sizeof(buf),0);
		printf("USER inpu Sound recoding\n");
	}
	fclose(sendf);
	fclose(recvf);
	close(sock);
	return 0;
}
