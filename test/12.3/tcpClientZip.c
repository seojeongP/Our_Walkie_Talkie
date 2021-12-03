#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include "quicklz.c"

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
	int i, sendBuf, recvBuf;
	char s_buf[5];
	char *send_buf_src, *send_buf_dst;
	char recv_buf_src[BUFSIZ], *recv_buf_dst;
	size_t in_f_len, out_f_len, in_b_len, out_b_len;

	qlz_state_compress *state_compress = (qlz_state_compress *)malloc(sizeof(qlz_state_compress));
	qlz_state_decompress *state_decompress =(qlz_state_decompress *)malloc(sizeof(qlz_state_decompress));
	
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
		//strcpy(buf, '\0');
		//memset(buf, '\0', BUF_SIZ);
		/*if((recvf = fopen("/home/jisong/sp2021-2/pro/recv.wav", "wb+")) == NULL){
                        perror("fopen");
                        exit(1);
                }*/
		//recv(sock, buf, BUF_SIZ, 0);
		printf("buf size : %ld\n", sizeof(buf));

		if((recvBuf = recv(sock, recv_buf_src, sizeof(recv_buf_src),0)) != -1){
			printf("recvBuf : %d\n", recvBuf);
			if((recvf = fopen("/home/jisong/sp2021-2/pro/recv.wav", "wb+")) == NULL){
                        	perror("fopen");
                        	exit(1);
                	}
			recv_buf_dst =(char *)malloc(recvBuf - 400);
			out_f_len = qlz_decompress(recv_buf_src,recv_buf_dst,state_decompress);

			if((i = fwrite(recv_buf_dst, out_f_len, 1, recvf))<=0)
			{
				perror("recv");
				exit(1);
			}
			printf("i : %d\n", i);
			
			system("aplay /home/jisong/sp2021-2/pro/recv.wav");
			printf("YOU: %s\n", "Sound recording");
		}
	//	system("aplay /home/jisong/sp2021-2/pro/recv.wav");
		
	//	printf("YOU: %s\n", "Sound recording");
		//memset(buf, '\0', BUF_SIZ);
		//strcpy(buf, '\0');

		fgets(s_buf, sizeof(s_buf), stdin);
		
		printf("USER INPUT: %s\n","Sound recording");
		if(!strcmp(s_buf,"s\n"))
		{
			system("arecord -d 5 /home/jisong/sp2021-2/pro/send.wav");
		}
		//memset(buf, '\0', BUF_SIZ);
		if((sendf = fopen("/home/jisong/sp2021-2/pro/send.wav","rb+")) == NULL){
			perror("fopen");
			exit(1);
		}
		fseek(sendf, 0, SEEK_END);
		in_f_len = ftell(sendf);
		fseek(sendf, 0, SEEK_SET);
		send_buf_src = (char *)malloc(in_f_len);

		if((c = fread(send_buf_src, 1, in_f_len, sendf))<=0){
			perror("fgets");
			exit(1);	
		}
		send_buf_dst =(char *)malloc(in_f_len+400);

		in_b_len =qlz_compress(send_buf_src, send_buf_dst, in_f_len, state_compress);
		
		sendBuf = send(sock,send_buf_dst,in_b_len,0);
		printf("sendBuf : %d\n", sendBuf);
		free(send_buf_src);
		free(send_buf_dst);
		free(recv_buf_dst);
	}
	free(state_decompress);
	free(state_compress);
	close(sock);
	fclose(sendf);
	fclose(recvf);
	return 0;
}
