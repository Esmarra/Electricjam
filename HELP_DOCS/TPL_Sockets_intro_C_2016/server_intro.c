//===================================================================
// program server_intro.c
/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
// tcp server(receives from and sends msg to client)
// usage (command line):  server_intro  portnumber
// changed by Paulo Coimbra, 13-10-21
//===================================================================
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
//===================================================================
// error messages: print message and terminate program
void error(char *msg) {
    perror(msg);
    exit(1);
}
//===================================================================
int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen;
    char buffer[256]; // data (bytes) to be read from client
    struct sockaddr_in serv_addr, cli_addr; //server, client address
    int n; //number of characters read or written from/to socket

    //---presentation msg...
    printf("\n\n\nProgram for sockets introduction (server)\n\n\n");

    //---check command parameters...
    if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
    }

    //---creates tcp socket (stream)...
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    //---binds socket to a specific portno (service)
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(serv_addr)) < 0) 
             error("ERROR on binding");

    //---listens for connection with, at most, 5 clients
    listen(sockfd,5);

    //---accept connection with client (waits for client...)
     clilen = sizeof(cli_addr); //client address lenght
     newsockfd = accept(sockfd, 
                 (struct sockaddr *) &cli_addr, 
                 &clilen);

     //---received contact (connect) from client...
     if (newsockfd < 0) error("ERROR on accept");

     //---reads data from socket (network)
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");

     //---prints message received...
     printf("Here is the message: %s\n",buffer);

     //---sends msg to client...
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");

     //---ends program
     return 0; 
}

//===================================================================
//===================================================================
