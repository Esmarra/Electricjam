/*=========================================
Program Name :	 rc_server
Base Language:	 Mixed English
Created by   :	 Esmarra
Creation Date:	 03/01/2018
Rework date entries:
Program Objectives:
Observations:
* Based on: server2_tcp by Paulo Coimbra, 2014-10-25
*
Special Thanks:
=========================================*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//==== DEFAULT SOCKETS ====//
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

//==== Functions ====//
void dostuff(int);
void error(char *msg);
//===================//

int main(int argc, char *argv[]) { // Call ./server2_tcp 50000
	int sockfd, newsockfd, portno, clilen, pid;
	struct sockaddr_in serv_addr, cli_addr;  // server addresses data
	//---presentation msg...
	printf("\n\n\nProgram for TCP sockets (server2)\n\n\n");
	//---check command parameters...
	if (argc < 2) {
		fprintf(stderr,"ERROR, no port provided\n");
		exit(1);
	}
	//==== Start Socket ===//
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)error("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if(bind(sockfd,(struct sockaddr *) & serv_addr,sizeof(serv_addr))<0)error("ERROR on binding");
	//==== Wait for Clients ===//
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	//==== Clients Loop ===//
	while(1){
		newsockfd = accept(sockfd,(struct sockaddr *) & cli_addr, &clilen); // Acept new clients
		if (newsockfd < 0) error("ERROR on accept");
		pid = fork();
		if (pid < 0) error("ERROR on fork");
		if (pid == 0) {  // child (new) process to attend client
			close(sockfd); // sockfd belongs to father process
			dostuff(newsockfd);
			exit(0);
		} else{ // parent (old) process that keeps wainting for clients
			close(newsockfd); // newsockfd belongs to child process
		}
	}
	return 0; /* we never get here */
}

//==== DO STUFF ====//
void dostuff (int sock) {
	int n;
	char buffer[256];
	//---reads and prints message from client..
	bzero(buffer,256);
	n = read(sock,buffer,255);
	
	//---reads and prints message from client...
	if (n < 0) error("ERROR reading from socket");
	printf("Here is the message: %s\n",buffer);

	//---sends message to client...
	n = write(sock,"I got your message",18);
	if (n < 0) error("ERROR writing to socket");
}
//==== ERROR ====//
void error(char *msg) { // error messages: print message and terminate program
	perror(msg);
	exit(1);
}
//===============//
