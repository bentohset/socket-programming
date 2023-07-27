/*******************************
udp_client.c: the source file of the client in udp
********************************/

#include "headsock.h"

void str_cli1(FILE *fp, int sockfd, struct sockaddr *addr, int addrlen, int *len);                

int main(int argc, char *argv[])
{
	int len, sockfd;
	struct sockaddr_in ser_addr;
	char **pptr;
	struct hostent *sh;
	struct in_addr **addrs;

	//check if command line arguments are given
	if (argc!= 2)
	{
		printf("parameters not match.");
		exit(0);
	}

	//get host's information and store in struct hostent
	if ((sh=gethostbyname(argv[1]))==NULL) {             
		printf("error when gethostbyname");
		exit(0);
	}

	//create socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);             
	if (sockfd<0)
	{
		printf("error in socket");
		exit(1);
	}
	//get host ip address
	addrs = (struct in_addr **)sh->h_addr_list;
	printf("canonical name: %s\n", sh->h_name);

	for (pptr=sh->h_aliases; *pptr != NULL; pptr++)
		printf("the aliases name is: %s\n", *pptr);			//printf socket information
	//print address type
	switch(sh->h_addrtype)
	{
		case AF_INET:
			printf("AF_INET\n");
		break;
		default:
			printf("unknown addrtype\n");
		break;
	}

	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(MYUDP_PORT);
	//copies address into ser_addr
	memcpy(&(ser_addr.sin_addr.s_addr), *addrs, sizeof(struct in_addr));
	bzero(&(ser_addr.sin_zero), 8);

	//call function to send packets
	str_cli1(stdin, sockfd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr_in), &len);   // receive and send

	close(sockfd);
	exit(0);
}

void str_cli1(FILE *fp, int sockfd, struct sockaddr *addr, int addrlen, int *len)
{
	char sends[MAXSIZE];	//buffer

	printf("Please input a string (less than 50 characters):\n");
	//reads in from file stream fp and stores into buffer
	if (fgets(sends, MAXSIZE, fp) == NULL) {
		printf("error input\n");
	}
	//send data to specific socket: server with destiantion address = addr
	sendto(sockfd, &sends, strlen(sends), 0, addr, addrlen);                         //send the packet to server
	printf("send out!!\n");
}
