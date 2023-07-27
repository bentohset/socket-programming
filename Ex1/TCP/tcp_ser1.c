/**********************************
tcp_ser.c: the source file of the server in tcp transmission
***********************************/


#include "headsock.h"

#define BACKLOG 10

void str_ser(int sockfd);                                                        // transmitting and receiving function
int main(void)
{
	int sockfd, con_fd, ret;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size;

	//process id of child service
	pid_t pid;

	//create socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);          
	if (sockfd <0)
	{
		printf("error in socket!");
		exit(1);
	}
	
	//server address structure defined
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYTCP_PORT);				//port number
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(my_addr.sin_zero), 8);

	//associate socket with my_addr
	ret = bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr));                //bind socket
	if (ret <0)
	{
		printf("error in binding");
		exit(1);
	}
	
	//wait for connection request from client
	//BACKLOG = max number of connections that can be queued up
	ret = listen(sockfd, BACKLOG);                           
	if (ret <0) {
		printf("error in listening");
		exit(1);
	}

	printf("receiving start\n");

	//receiving packets
	while (1)
	{
		sin_size = sizeof (struct sockaddr_in);
		//synchronously extracts first pending connection request from queue, returns new socket
		con_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);            //accept the packet
		if (con_fd <0)
		{
			printf("error in accept\n");
			exit(1);
		}
		//child process created to handle new connetion request, if pid==0 then is child process
		if ((pid = fork())==0)                                         // creat acception process
		{
			close(sockfd);
			//recevie data from con_fd specific socket
			str_ser(con_fd);                                          //receive packet and response
			close(con_fd);
			exit(0);
		}
		//if pid > 0, it is parent proces and closes socket and listens for more connections
		else close(con_fd);                                         //parent process
	}
	close(sockfd);
	exit(0);
}

void str_ser(int sockfd)
{
	char recvs[MAXSIZE];
	int n = 0;

	//receive data from connected socket
	if ((n= recv(sockfd, &recvs, MAXSIZE, 0))==-1)                                   //receive the packet
	{
		printf("receiving error!\n");
		return;
	}
	recvs[n] = '\0';
	printf("the received string:\n%s\n", recvs);
}
