#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "quicklz.c"

#define PORTNUM 9001
#define BUF_SIZ 80044

int system(const char* string);
int main(void){
	char buf[BUF_SIZ];
	struct sockaddr_in sin, cli;
	FILE *sendf, *recvf;
	int i;
	int c;
	char s_buf[5];
	char *send_buf_src, *send_buf_dst;
	char recv_buf_src[BUFSIZ], *recv_buf_dst;
	size_t in_f_len, in_b_len;
	size_t out_f_len, out_b_len;
	qlz_state_compress *state_compress = (qlz_state_compress *)malloc(sizeof(qlz_state_compress));
	qlz_state_decompress *state_decompress = (qlz_state_decompress *)malloc(sizeof(qlz_state_decompress));
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
	sin.sin_addr.s_addr = inet_addr("192.168.0.6");

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
		//memset(buf, '\0',BUF_SIZ);
		fgets(s_buf, sizeof(s_buf), stdin);
		
	//	n = read(comm_socket,buf,BUFFER_SIZE);
//		printf("Recieved file name : %s\n",buf);
//		if(fp == NULL)
//			printf("file open failed!\n");
//		else
//			printf("File successfully opened!\n");
//		size_t num_read = fread(buf, 1, BUFFER_SIZE, fp);
		
//		n = write(comm_socket, buf, num_read);

		if(!strcmp(s_buf, "s\n")){
			system("arecord -d 5 /home/seojeong/Desktop/systemprogramming/test.mp3");
			//sleep(6); //6sec
	//		fp = fopen(buf, "rb");
		}
		printf("USER INPUT: %s\n", "Sound recording");

		if((sendf = fopen("/home/seojeong/Desktop/systemprogramming/test.mp3", "rb+")) == NULL){
			perror("fopen: test.wav");
			exit(1);
		}
		
		fseek(sendf, 0, SEEK_END);
		in_f_len = ftell(sendf);
		fseek(sendf, 0, SEEK_SET);

		send_buf_src = (char *)malloc(in_f_len);

		if((c = fread(send_buf_src,  1 , in_f_len, sendf)) <= 0){
			perror("get file");
			exit(1);
		}

		send_buf_dst = (char *)malloc(in_f_len + 400);

		in_b_len = qlz_compress(send_buf_src, send_buf_dst, in_f_len, state_compress);

		printf("c : %d\n", c);
		printf("strlen(buf) : %ld\n", strlen(buf)+1);
		printf("sizeof(buf) : %ld\n", sizeof(buf));

		int ab;
		ab  = send(comm_socket, send_buf_dst, in_b_len, 0);
		printf("ab : %d\n", ab);

		//memset(buf, '\0', BUF_SIZ);
		int ba;
		//recv(comm_socket, buf, BUF_SIZ, 0);
		if(ba = recv(comm_socket, recv_buf_src, sizeof(recv_buf_src), 0) != -1){
			printf("ba : %d\n", ba);
			if((recvf = fopen("/home/seojeong/Desktop/systemprogramming/out.mp3", "wb+")) == NULL){
				perror("fopen: out.mp3");
				exit(1);
			}

			recv_buf_dst = (char *)malloc(ba-400);
			
			out_f_len = qlz_decompress(recv_buf_src, recv_buf_dst, state_decompress);

			if((i = fwrite(recv_buf_dst, out_f_len ,1, recvf)) <= 0){
				perror("put file");
				exit(1);
			}
			system("aplay /home/seojeong/Desktop/systemprogramming/out.mp3");
                	printf("YOU: %s\n", "Sound recording");
		
		}
		//system("aplay /home/seojeong/Desktop/systemprogramming/out.mp3");
                //printf("YOU: %s\n", "Sound recording");
		
		free(send_buf_src);
		free(send_buf_dst);
		free(recv_buf_dst);

	}

//	if(fp != NULL)
//		fclose(fp);

	free(state_decompress);
	free(state_compress);
	fclose(sendf);
	fclose(recvf);
	close(access_socket);
	close(comm_socket);

	return 0;
}
