//===================================================================
// program server2_tcp
// tcp server (receives from and sends msg to client)
// usage (command line):  server2_tcp  portnumber
// changed by Paulo Coimbra, 2014-10-25
/* A simple server in the internet domain using TCP
   The port number is passed as an argument 
   This version runs forever, forking off a separate 
   process for each connection
*/
//===================================================================
#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
//===================================================================
void dostuff(int); /* function prototype */
//===================================================================
// error messages: print message and terminate program
void error(char *msg) {
    perror(msg);
    exit(1);
}
//===================================================================
int main(int argc, char *argv[]) {
    int sockfd, newsockfd, portno, clilen, pid;
    struct sockaddr_in serv_addr, cli_addr;  //server addresses data

    //---presentation msg...
    printf("\n\n\nProgram for TCP sockets (server2)\n\n\n");

    //---check command parameters...
    if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

    //---creates tcp welcome socket (stream)...
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
                  error("ERROR on binding");
	//---waits for clients
     listen(sockfd,5); 
     clilen = sizeof(cli_addr);

	//---forever cicle for clients
     while (1) {
	    //---accepts a new client
         newsockfd = accept(sockfd, 
               (struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) error("ERROR on accept");
         pid = fork();
         if (pid < 0) error("ERROR on fork");
         if (pid == 0) {  // child (new) process to attend client
             close(sockfd); // sockfd belongs to father process
             dostuff(newsockfd);
             exit(0);
         } else // parent (old) process that keeps wainting for clients
             close(newsockfd); // newsockfd belongs to child process
     } //while
     return 0; /* we never get here */
}
//===================================================================
/******** DOSTUFF() *********************
 There is a separate instance of this function 
 for each connection.  It handles all communication
 once a connnection has been established.
 *****************************************/
void dostuff (int sock) {
   int n;
   char buffer[256];
      
   //---reads and prints message from client...
   bzero(buffer,256);
   n = read(sock,buffer,255);
   if (n < 0) error("ERROR reading from socket");
   printf("Here is the message: %s\n",buffer);

   //---sends message to client...
   n = write(sock,"I got your message",18);
   if (n < 0) error("ERROR writing to socket");
}
//===================================================================
//===================================================================

