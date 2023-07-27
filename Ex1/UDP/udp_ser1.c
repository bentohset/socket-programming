/**************************************
udp_ser.c: the source file of the server in udp transmission
**************************************/
#include "headsock.h"

void str_ser1(int sockfd);                                                           // transmitting and receiving function

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in my_addr;

	//create socket with AF_INET internet protocol and SOCK_DGRAM for UDP
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {			//create socket
		printf("error in socket");
		exit(1);
	}

	//server address structure defined
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYUDP_PORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	bzero(&(my_addr.sin_zero), 8);

	//associate socket with address and port number in my_addr
	if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) {           //bind socket
		printf("error in binding");
		exit(1);
	}

	printf("start receiving\n");

	//repeatedly receive data packet from client
	while(1) {
		str_ser1(sockfd);                        // send and receive
	}
	close(sockfd);
	exit(0);
}

void str_ser1(int sockfd)
{	
	//buffer to store received data
	char recvs[MAXSIZE];
	int n = 0, len;
	struct sockaddr_in addr;

	len = sizeof (struct sockaddr_in);

	//receive data packet from client specific socket
	if ((n=recvfrom(sockfd, &recvs, MAXSIZE, 0, (struct sockaddr *)&addr, &len)) == -1) {      //receive the packet
		printf("error receiving");
		exit(1);
	}
	recvs[n] = '\0';
	printf("the received string is :\n%s", recvs);
}
