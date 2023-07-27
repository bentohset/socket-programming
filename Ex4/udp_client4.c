#include "headsock.h"

#define MAX_DUS 3

float str_cli(FILE *fp, int sockfd, struct sockaddr *addr, int addrlen, long *len);
void tv_sub(struct timeval *out, struct timeval *in);

int main(int argc, char *argv[]) {
    int sockfd;
    long len;
    struct sockaddr_in ser_addr;
    float ti, rt;
    char **pptr;
    struct hostent *sh;
	struct in_addr **addrs;
    FILE *fp;

    if (argc!= 2) {
		printf("parameters not match.");
		exit(0);
	}

    //get host's information
    sh = gethostbyname(argv[1]);	                                       
	if (sh == NULL) {
		printf("error when gethostby name");
		exit(0);
	}

    //create socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd <0) {
		printf("error in socket");
		exit(1);
	}

    //get host ip address
	addrs = (struct in_addr **)sh->h_addr_list;
    printf("canonical name: %s\n", sh->h_name);

    //printf socket information
	for (pptr=sh->h_aliases; *pptr != NULL; pptr++) {
		printf("the aliases name is: %s\n", *pptr);			
    }

    //print address type
	switch(sh->h_addrtype) {
		case AF_INET:
			printf("AF_INET\n");
		break;
		default:
			printf("unknown addrtype\n");
		break;
	}

    //define server address structure
    ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(MYUDP_PORT);
    //copies address into ser_addr
    memcpy(&(ser_addr.sin_addr.s_addr), *addrs, sizeof(struct in_addr));
	bzero(&(ser_addr.sin_zero), 8);


    if((fp = fopen ("myfile.txt","r+t")) == NULL) {
		printf("File doesn't exit\n");
		exit(0);
	}

    //calculate stats, ti = time taken, rt = average transmission rate
    ti = str_cli(fp, sockfd, (struct sockaddr *)&ser_addr, sizeof(struct sockaddr_in), &len);
	rt = (len/(float)ti);
	printf("Time(ms) : %.3f\nData sent(byte): %d\nData rate: %f (Kbytes/s)\n", ti, (int)len, rt);

    close(sockfd);
    fclose(fp);
    exit(0);
}

float str_cli(FILE *fp, int sockfd, struct sockaddr *addr, int addrlen, long *len) {
    char *buf;
    char send[DATALEN];
    long lsize, ci;
    struct ack_so ack;
    int n, slen;
    float time_inv = 0.0;
    struct timeval sendt, recvt;
    ci = 0;

    int count = 1;
    int du = 0;
    uint8_t ackNum = 0;

    //retrieve file data
    fseek(fp , 0 , SEEK_END);
	lsize = ftell (fp);
	rewind (fp);
	printf("The file length is %d bytes\n", (int)lsize);
	printf("the packet length is %d bytes\n\n",DATALEN);

    //allocate memory to contain the whole file.
    buf = (char *) malloc (lsize);
	if (buf == NULL) exit (2);

    //copy the file into the buffer
	fread (buf,1,lsize,fp);
    buf[lsize] = '\0';

    gettimeofday(&sendt, NULL);

    //ci = current index of sent bytes
    //while current index is less than the total size in msg file
    while (ci <= lsize) {
        if ((lsize + 1 - ci) <= DATALEN) slen = lsize + 1 - ci;
        else slen = DATALEN;

        //copy buffer into sending packet
        memcpy(send, (buf+ci), slen);

        //send to server
        n = sendto(sockfd, &send, slen, 0, addr, addrlen);
        if (n == -1) {
            printf("send error");
            exit(1);
        }
        du++;
        ci += slen;
        
        //printf("DU = %d \n", du);

        //if DU finishes the cycle, wait for ACK and increment/rollback counter
        if (du == count) {
            printf("count = %d \n", count);
            //wait for ack
            n = recvfrom(sockfd, &ack, 2, 0, addr, (socklen_t*)&addrlen);
            if (n == -1) {
                printf("error when receiving\n");
                exit(1);
            }
            //check ACK correctness
            ackNum++;
            printf("ACK %d received\n", ack.num);
            printf("ACK %d expected\n", ackNum);
            if (ack.num != ackNum || ack.len != 0) {
                printf("error in transmission\n");
                exit(0);
            }
            //no error
            count = (count == 3) ? 1 : count+1;
            du = 0;
            printf("\n");
        }

        //indicates if last byte is sent
        if (buf[ci] == '\0') {
            printf("last byte sent out\n\n");
        }

    }
    //printf("finished sending\ncalculating stats...\n\n");

    //calculate time taken
    gettimeofday(&recvt, NULL);
    //get current time
	*len= ci;
	tv_sub(&recvt, &sendt);

    //get the whole trans time 
	time_inv += (recvt.tv_sec)*1000.0 + (recvt.tv_usec)/1000.0;
	return(time_inv);
}

void tv_sub(struct  timeval *out, struct timeval *in)
{
	if ((out->tv_usec -= in->tv_usec) <0)
	{
		--out ->tv_sec;
		out ->tv_usec += 1000000;
	}
	out->tv_sec -= in->tv_sec;
}