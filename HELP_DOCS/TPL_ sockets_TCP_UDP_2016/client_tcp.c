//===================================================================
// program client_tcp (equal to intro.cclient_intro.c)
// tcp client (sends to and receive msg from server)
// usage (command line):  client_tcp  hostname  portnumber
// changed by Paulo Coimbra, 11.10.25, 2013-10-21
//===================================================================
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
//===================================================================
// error messages: print message and terminate program
void error(char *msg) {
    perror(msg);
    exit(0);
}
//===================================================================
int main(int argc, char *argv[]) {
    int sockfd, portno, n;  //socket file descriptor, port number
    struct sockaddr_in serv_addr;  //server address data
    struct hostent *server;
    char buffer[256];  // data (bytes) to be sent to server

    //---presentation msg...
    printf("\n\n\nProgram for sockets introduction (client)\n\n\n");

    //---check command parameters...
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    //---creates tcp socket (stream)...
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    server = gethostbyname(argv[1]);  //uses DNS to know IP address
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr)); //clears serv_addr
    serv_addr.sin_family = AF_INET; 
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno); //portno must be in network format

    //---connect (to server)
    if (connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    //---ask for message to send...
    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);

    //---sends message to server...
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) error("ERROR writing to socket");

    //---reads and prints message from server...
    bzero(buffer,256);
    n = read(sockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("%s\n",buffer);

     //---ends program
    return 0;
}
//===================================================================
//===================================================================

