#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>

static int socketfd,clientfd;

struct sockaddr_in server_addr,client_addr;

char recv_buf[1024] = {0};
char path[1024] = {0};
void hander_client(int clientfd_new) {
	int ret = read(clientfd_new,recv_buf,sizeof(recv_buf));
	if(ret < 0) {
		printf("recv failed\n");
		return 0;
	}

	if(strstr(".html",)) {
	
	} 
}
int main() {
	
	socketfd = socket(AF_INET,SOCKET_STREAM,0);
	if(socketfd < 0) {
		perror("socketfd create failed\n");
		exit(EXIT_FAILURE);	
	}
	
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8080);
	server_addr.in_addr.s_addr = INADDR_ANY;
	
	if(bind(socketfd,(struct sockaddr*)&server_addr,sizeof(server_addr) < 0)) {
		close(socketfd);
		perror("sock bind failed\N");
		exit(EXIT_FAILURE);
	}

	if(listen(socketfd, 5) < 0) {
		close(socketfd); 
		perror("listen failed\n");
		exit(EXIT_FAILURE);
	}

	while(1) {
		clientfd = accept(socketfd,(struct sockaddr_in *)&client_addr,sizeof(client_addr));
		if(clientfd < 0) {
			perror("aceept failed \n");
			exit(EXIT_FAILURE);
		}

		hander_client(clientfd);
	}
	return 0;

}
