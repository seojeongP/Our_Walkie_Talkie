#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define PORTNUM 9001
#define BUF_SIZ 644000

int mutex = 1;

struct sock{
	int conn_socket;
};

void *recv_method(void *filename);

int system(const char* string);

int main(void){
	char buf[BUF_SIZ], s_buf[10], len_buf[10], null_buf[5];
	char filename[BUF_SIZ];
	struct sockaddr_in sin, cli;
	FILE *sendf, *recvf;
	int i, in_f_len, songnum;
	int c;
	int access_socket, comm_socket;
	int clientlen = sizeof(cli);
	int serverlen = sizeof(sin);

	// pthread_t create
	pthread_t recv_m;

	if((access_socket = socket(AF_INET,SOCK_STREAM,0)) ==-1){
		perror("socket");
		exit(EXIT_FAILURE);
	}

	memset((char *)&sin, '\0',serverlen);
	sin.sin_family =AF_INET;
	sin.sin_port = htons(PORTNUM);
	sin.sin_addr.s_addr = inet_addr("192.168.0.105");

	if(bind(access_socket,(struct sockaddr *)&sin, serverlen)){
		perror("bind");
		exit(EXIT_FAILURE);
	}
	if(listen(access_socket, 5)){
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if((comm_socket =accept(access_socket, (struct sockaddr *)&cli,&clientlen)) ==-1){
		perror("accept");
		exit(EXIT_FAILURE);
	}

	printf("Welcome to Our Walkie Talkie :) !!! \n");
	
	struct sock c_sock = {comm_socket};
	
	while(1){
		
		printf("You sound recording\n");
		memset(s_buf, '\0', sizeof(s_buf));
		printf("Enter the s(record), m(music), q(quit) : ");
       		fgets(s_buf, sizeof(s_buf), stdin);
        	
		mutex = 0;
		if(!strcmp(s_buf, "s\n")){
			printf("USER INPUT: Sound recording");
                	system("arecord -d 5 /home/seojeong/Desktop/systemprogramming/12.4/test.wav");
			strcpy(filename, "/home/seojeong/Desktop/systemprogramming/12.4/test.wav");
        	}

		if(!strcmp(s_buf, "m\n")){
			printf("=========================\n");
			printf("choose song !\n");
			printf("1.sokodomo\n2.mudd the student\n3.aespa\n");
			printf("Enter the song number : ");
			scanf("%d", &songnum);
			printf("=========================\n");

			switch(songnum){
				case 1:
					strcpy(filename, "/home/seojeong/Desktop/systemprogramming/12.4/merry.wav");
					break;
				case 2:
					strcpy(filename, "/home/seojeong/Desktop/systemprogramming/12.4/mudd.wav");
                                        break;
				case 3:
					strcpy(filename, "/home/seojeong/Desktop/systemprogramming/12.4/savage.wav");
                        }
		}

		if(!strcmp(s_buf, "q\n")){
			printf("Thank you for using program !! :-)\n");
			exit(1);
		}

	        if((sendf = fopen(filename, "rb+")) == NULL){
			perror("fopen: test.wav");
               		exit(1);
		}
		
		//send file size
		fseek(sendf, 0, SEEK_END);
		in_f_len = ftell(sendf);
		fseek(sendf, 0, SEEK_SET);

		//file size send
		sprintf(len_buf, "%d", in_f_len);
		printf("what is in len_buf : %s\n", len_buf);
		send(comm_socket, len_buf, sizeof(len_buf), 0);

		memset(buf, '\0', in_f_len);
		//send file content
		if((c = fread(buf,  in_f_len, 1, sendf)) <= 0){
                	perror("get file");
                	exit(1);
        	}

		int ab;
        	ab  = send(comm_socket, buf, in_f_len, 0);
        	printf("ab : %d\n", ab);

		mutex = 1;

		printf("please enter any key\n");
		fgets(null_buf, sizeof(null_buf), stdin);


		pthread_create(&recv_m, NULL, recv_method, (void *)&c_sock);
		sleep(10);
	}
	pthread_join(recv_m, NULL);
	fclose(sendf);
	fclose(recvf);
	close(access_socket);
	close(comm_socket);

	return 0;
}

void *recv_method(void *sock){
	printf("in here thread?\n");
	struct sock* c_sock = (struct sock*)sock;
	int comm_socket = c_sock->conn_socket;
	char buf[BUF_SIZ];
	int i, f_len;
	FILE *recvf;

	while(1){/*
		recv(comm_socket, buf, BUF_SIZ, 0);
		f_len = atoi(buf);
		printf("f_len : %d\n", f_len);
*/
		if(mutex == 1){
			if(recv(comm_socket, buf, f_len, 0) != -1){
				//printf("iiiiin here?\n");
                       		if((recvf = fopen("/home/seojeong/Desktop/systemprogramming/12.4/recv.wav", "wb+")) == NULL){
                                	perror("fopen: recv.mp3");
                                	exit(1);
        	                }
	
                	        if((i = fwrite(buf, f_len,1, recvf)) <= 0){
                       	        	perror("put file");
                                	exit(1);
                        	}
                	}
                	system("aplay /home/seojeong/Desktop/systemprogramming/12.4/recv.wav");
		}
	}
}
