modified version of ex3
use udp instead - see ex1 for differences between udp and TcP
-increment DU out after each ACK

1 -> 2 -> 3 -> 1 -> 2
loop

tcp: uses send() and recv(), connetion oriented
udp: uses sendto() and recvfrom()

compile gcc:
gcc -g -Wall -o FILENAME SCRIPT.c

gcc -g -Wall -o udp_client4 udp_client4.c
gcc -g -Wall -o udp_client4p2 udp_client4p2.c

gcc -g -Wall -o udp_ser4 udp_ser4.c
gcc -g -Wall -o udp_ser4p2 udp_ser4p2.c
