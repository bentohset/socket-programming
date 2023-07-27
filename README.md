# Socket Programming Exercises
Some exercises for socket programming in C learnt in EE4204.

To compile the files, use WSL or a VM if on Windows.

```
gcc –g –Wall -o hello hello.c
```

- -g enables debugging
- -Wall turns on warnings
- -o file Place output in file file

To execute, just run the compiled files with `./hello`

To check differences:
```
diff myfile.txt myUDPreceive.txt
```


## Exercise 1
A socket program that uses TCP and UDP as the transport protocol for transferring a short message between a client and server. The client sends a string (input by the user) to the server and the server prints the string on the screen after receiving it.


## Exercise 2
A TCP-based client-server socket program for transferring a large message. The entire message is sent by the client as a single data-unit. After receiving the file, the server sends an ACK message to the receiver. Measure the message transfer time and throughput.

The large message is read from a file, which has nearly 50000 bytes in the test (if larger size is desired, the MAXLEN in “headsock.h” should be also modified). The client sends the entire message to the server in one data-unit (let us call it a packet).
TCP ensures that the entire message is received by the receiver correctly and in order with no errors. In the program, the function “recv” is called repeatedly in the serve side until all the data has been received. The received data is stored in file "myTCPreceive.txt".


## Exercise 3
A TCP-based client-server socket program for transferring a large message. 

The message transmitted from the client to server is read from a large file. The message is split into short data-units which are sent one by one without waiting for any acknowledgement between transmissions of two successive data-units. Measure the message transfer time and throughput for various sizes of data-units.

The packet size is fixed at 100 bytes per packets. The sender does not stop its sending until all the data has been sent out. The program doesn’t use the packet structure as last section.
The receiver will check the last byte in the packet it received to see whether the transmission has ended. If true, it sends back an acknowledgement to the sender (i.e. client). 
The sender will calculate the transfer time after receiving the acknowledgement. (In standard packet structure, some information to aid error /flow control is included. But in this program, there is no feedback information to the sender, we simply ignore the above information, and the packet contains the data to transmit only.)


## Exercise 4
UDP-based client-server socket program for transferring a large message using a varying-batch-size protocol. 

The message transmitted from the client to server is read from a large file. The message is split into short data-units (DUs) which are sent and acknowledged in batches of size 1, 2, and 3 DUs. The sender sends one DU, waits for an acknowledgment (ACK); sends two DUs, waits for an ACK; sends three DUs, waits for an ACK; and repeats the above procedure until the entire file is sent. The receiver sends an ACK after receiving a DU; sends next ACK after receiving two DUs; sends next ACK after receiving three DUs; and repeats the above procedure.

## Documents
- Lab Report and results
- Course Material
- Basic instructions