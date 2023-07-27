#include "headsock.h"

void str_ser(int sockfd, struct sockaddr *addr, int addrlen);

//create socket with server addresses and port, poll for data
int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in my_addr;

    //create socket, AF_INET ip, SOCK_DGRAM udp, 
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        printf("error in socket");
        exit(1);
    }

    //server address structure definition
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYUDP_PORT);
    my_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(my_addr.sin_zero), 8);

    //associate socket with address and port number in my_addr
    if (bind(sockfd, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1) {
        printf("error in binding");
        exit(1);
    }
    printf("start receiving\n");

    str_ser(sockfd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr_in));
    close(sockfd);
    exit(0);

}

void str_ser(int sockfd, struct sockaddr *addr, int addrlen) {
    char recvs[DATALEN];
    char buf[BUFSIZE];
    struct ack_so ack;
    ack.num = 0;
    FILE *fp;
    int end = 0;
    int n = 0;
    long lseek = 0;

    //count = varying batch size (1 to 3), du = current data unit
    int count = 1;
    int du = 0;

    while(!end) {
        n = recvfrom(sockfd, &recvs, DATALEN, 0, addr, (socklen_t *)&addrlen);
        if (n == -1) {
            printf("error receiving");
            exit(1);
        }

        du++;

        if (recvs[n-1] == '\0') {
            printf("received last byte\n");
            end = 1;
            n--;
        }
        //copy received into buffer
        memcpy((buf+lseek), recvs, n);
        lseek += n;

        printf("Received= %d \n", count);
        //send ACK when all DU (1,2,3) is received
        if (du == count || end == 1) {
            ack.num += 1;
            ack.len = 0;
            n = sendto(sockfd, &ack, 2, 0, addr, addrlen);
            if (n == -1) {
                printf("error sending ACK\n");
                exit(1);
            }

            printf("ACK %d sent\n", ack.num);

            //reset DU and increment/rollback counter
            du = 0;
            count = (count == 3) ? 1 : count+1;
        } 
    }
    printf("\nfinished receiving!\n\n");

    if ((fp = fopen ("myUDPreceive.txt","wt")) == NULL)
    {
        printf("File doesn't exit\n");
        exit(0);
    }
    //write from buffer to file
    fwrite (buf, 1 , lseek , fp);	
    fclose(fp);
    printf("a file has been successfully received!\nthe total data received is %d bytes\n", (int)lseek);
}