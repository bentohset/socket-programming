/**********************************
tcp_ser.c: the source file of the server in tcp transmission for a large packet
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
	pid_t pid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);          //create socket
	if (sockfd <0)
	{
		printf("error in socket!");
		exit(1);
	}
	
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYTCP_PORT);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&(my_addr.sin_zero), 8);


	ret = bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr));                //bind socket
	if (ret <0)
	{
		printf("error in binding");
		exit(1);
	}
	
	ret = listen(sockfd, BACKLOG);                              //listen if there is connection
	if (ret <0) {
		printf("error in listening");
		exit(1);
	}
	printf("receiving start\n");
	while (1)
	{
		sin_size = sizeof (struct sockaddr_in);
		con_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);            //accept the packet

		if (con_fd <0)
		{
			printf("error in accept\n");
			exit(1);
		}

		if ((pid = fork())==0)                                         // creat acception process
		{
			close(sockfd);
			str_ser(con_fd);                                          //receive packet and response
			close(con_fd);
			exit(0);
		}
		else close(con_fd);                                         //parent process
	}
	close(sockfd);
	exit(0);
}

//difference
void str_ser(int sockfd)
{
	char buf[BUFSIZE];
	FILE *fp;
	//data structure for packet and acknowledgement
	struct pack_so recvs;
	struct ack_so ack;
	//n = no. of bytes received
	//end of received packet
	int end, n = 0, ci, lsize=1;
	//current index
	//lsize = size of received data
	ci = end = ack.num = 0;

	while(ci < lsize)
	{
		if ((n= recv(sockfd, &recvs, MAXSIZE, 0))==-1)                                   //receive the packet
		{
			printf("receiving error!\n");
			return;
		}
		else printf("%d data received\n", n);
		//if it is the first packet received
		if (ci == 0) {
			lsize = recvs.len;								//copy the data length
			memcpy(buf, recvs.data, (n-HEADLEN));			//copy the data
			ci += n-HEADLEN;		//add to index the number of data received
		}
		//if not first -> wont execute as this program sends data in a single unit
		else {
			memcpy((buf+ci), &recvs, n);		//copy entire packet into buffer after ci
			ci += n;
		}
	}
	//set ACK parameters
	ack.len = 0;
	ack.num = 1;
//	memcpy(buf, recvs.data, recvs.len);
	send(sockfd, &ack, 2, 0);                                                  //send ACK or NACK

	if((fp = fopen ("myTCPreceive.txt","wt")) == NULL)
	{
		printf("File doesn't exit\n");
		exit(0);
	}
	printf("the data received: %d\n", ci);
	printf("the file size received: %d\n", lsize);
	fwrite (buf , 1 , lsize, fp);								//write the data into file
	fclose(fp);
	printf("a file has been successfully received!\n");
}
