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
#include <stdlib.h> // strcat
#include <string.h> // strcmp
//==== DEFAULT SOCKETS ====//
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Read ip ( inet_ntoa )
#include <netdb.h>
//========================//
#define MAX_CAR 100
#define MAX_EVENT_NUM 20

char read_file_name[]="sauce/Events_List.txt";   // Read event file located at /sauce/
char *client_ip; // Stores Client IP - Create client structure?

struct Events{ // Stores Events in struct (Used on Server & Client Side)
  char name[MAX_EVENT_NUM][MAX_CAR];
  int num_event;// Number of Events in List
};

//==== Functions ====//
void error(char *msg);
//===================//

int main(int argc, char *argv[]) { // Call ./udp_server 50000
  //==== Add Code ====//
  printf("\n Modded File\n");
  struct Events event; // Start struct
  event.num_event=0; // Init zeros

  //==== READ .TXT FILE ====// Guarda todos Eventos para uma estrutura (fazer isto no server side?)
  FILE *ficheiro1;
  ficheiro1 = fopen(read_file_name,"rt"); // Inicializa ficheiro de leitura
  while (fgets(event.name[event.num_event], MAX_CAR, ficheiro1) != NULL){ // Le ficheir linha a linha
    //printf("Event Num:%d | Name: %s", event.num_event,event.name);
    event.num_event++; // Incrementa o numero de enventos
  }
  event.num_event-=1; // Last is <null>
  fclose(ficheiro1);// Close file
  //=======================//

  int sock, length, fromlen, n;
  struct sockaddr_in server;
  struct sockaddr_in from;
  char buf[1024];
  char text_msg[256];// message (text) to send

  if (argc < 2) {
     fprintf(stderr, "ERROR, no port provided\n");
     exit(0);
  }

  //==== Start Socket Datagram (UDP) ===//
  sock=socket(AF_INET, SOCK_DGRAM, 0);
  if (sock < 0) error("Error in opening socket");
  length = sizeof(server);
  bzero(&server,length);
  server.sin_family=AF_INET;
  server.sin_addr.s_addr=INADDR_ANY;
  server.sin_port=htons(atoi(argv[1]));
  if (bind(sock,(struct sockaddr *)&server,length)<0) error("Error in binding");
  fromlen = sizeof(struct sockaddr_in);

  //forever cicle for clients
  while (1) {
      write(1,"Waiting for a client...",23);
      //receives datagrama that identifies client
      n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
      if (n < 0) error("Error in recvfrom");
      write(1,"Received a datagram: ",21);
      write(1,buf,n);
      printf("buf:%s!\n",buf );
      int i;
      i=0;
      i=atoi(buf);

      if(i==1)printf(" It's me One :D \n" );
      printf("Number:%d!\n",i );

      //printf("Host: %s\n",(struct hostent *) h_name );hostent
      //printf("IP:%s\n",inet_ntoa(from.sin_addr)); // Display Client IP [DEBUG]
      strcpy(&client_ip, inet_ntoa(from.sin_addr)); // Stores Client IP Right Practice

      //n = sendto(sock, &i, sizeof(i),0, (struct sockaddr *)&from, fromlen);
      //if (n < 0) error("Error in sendto");
      //sends response text
      //n = sendto(sock,&client_ip,sizeof(&client_ip),0,(struct sockaddr *)&from,fromlen);
      //if (n  < 0) error("Error sending client_ip");


      //i=ntohl(i);
      //printf(“%d”, i);

      n = sendto(sock,"Got your message\n",17,0,(struct sockaddr *)&from,fromlen);
      if (n < 0) error("Error in sendto");
  }//while
}

//==== ERROR ====//
void error(char *msg) { // error messages: print message and terminate program
	perror(msg);
	exit(1);
}
//===============//
