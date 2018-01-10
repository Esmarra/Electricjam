/*=========================================
Program Name :	 udp_server
Base Language:	 English
Created by   :	 Esmarra
Creation Date:	 03/01/2018
Rework date entries:
* 05/01/2018
* 06/01/2018
* 09/01/2018
* 10/01/2018
Program Objectives:
Observations:
* Based on: server2_tcp by Paulo Coimbra, 2014-10-25
* Git Repo: https://github.com/Esmarra/Electricjam
Special Thanks:
=========================================*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h> // strcat
#include <string.h> // strcmp
#include <time.h> // Add time stamp to Registration
//==== DEFAULT SOCKETS ====//
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // Read ip ( inet_ntoa )
#include <netdb.h>
//========================//
#define MAX_CAR 100 // Max strlen event name can have (REMOVE FOR CLIENT?)
#define MAX_EVENT_NUM 20 // Max Number of Events
#define MAX_USERNAME 20 //Max size of username

char read_file_name[]="sauce/Events_List.txt";   // Read event file located at /sauce/
char *client_ip; // Stores Client IP - Create client structure?
char *reg_file[10]; //Name if the Reg file for each event
char username[MAX_USERNAME]; //Username

struct Events{ // Stores Events in struct (Used on Server & Client Side)
  char name[MAX_EVENT_NUM][MAX_CAR]; // Stores envent name
  int num_event;// Number of Events in List
};

//==== Functions ====//
void error(char *msg);
//===================//

int main(int argc, char *argv[]) { // Call ./udp_server 50000
  time_t mytime; //Log time on files
  //==== Resitration Per Event File ====//
  reg_file[0] = "sauce/Reg_List_Evt1.txt";
  reg_file[1] = "sauce/Reg_List_Evt2.txt";
  reg_file[2] = "sauce/Reg_List_Evt3.txt";
  reg_file[3] = "sauce/Reg_List_Evt4.txt";
  reg_file[4] = "sauce/Reg_List_Evt5.txt";
  reg_file[5] = "sauce/Reg_List_Evt6.txt";
  reg_file[6] = "sauce/Reg_List_Evt7.txt";
  reg_file[7] = "sauce/Reg_List_Evt8.txt";
  reg_file[8] = "sauce/Reg_List_Evt9.txt";
  reg_file[9] = "sauce/Reg_List_Evt10.txt";
  //====================================//

  //==== Add Code ====//
  printf("\n Modded File\n");
  struct Events event; // Start struct
  event.num_event=0; // Init zeros
  int ev_reg_num;
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
      write(1,"S:Received a datagram: ",23);
      write(1,buf,n);
      strtok(buf, "\n"); // Removes \n from string (NOT Optimal, but it works)
      printf("\n Server Buffer>%s<",buf ); //Display whats in the buffer [DEBUG]

      if(strcmp(buf,"n_event")==0){ //Repplys Number of envents
        printf("\nnum event:%d\n",event.num_event );
        // I Can't use little endian, so
        char temp[]=""; // Char to send Int
        sprintf(temp,"%d",event.num_event); // Int to str
        n = sendto(sock,temp,strlen(temp),0,(struct sockaddr *)&from,fromlen);
        if (n < 0) error("Error sending num_event");
      }

      if(strcmp(buf,"n_usere")==0){//Enter User Validation Mode
        printf("\n Waiting for username");
        bzero(buf,256);
        n = recvfrom(sock,buf,256,0,(struct sockaddr *)&from,&fromlen);//Get Username from client
        strcpy(username,buf);
        printf("UserName=%s\n",username); //[DEBUG]
      }

      if(strcmp(buf,"d_event")==0){
        printf("\n Displaying Events:\n");
        // send num event to server first
        int i;
		    for(i=0;i<event.num_event;i++){
    			printf("  No%d, Event: %s",i,event.name[i]);
          n = sendto(sock,event.name[i],sizeof(event.name[i]),0,(struct sockaddr *)&from,fromlen);
          if (n < 0) error("Error sending events name");
		    }
      }

      if(strcmp(buf,"n_regis")==0){
        printf("\n Waiting for client to Register");
        bzero(buf,256);
        n = recvfrom(sock,buf,256,0,(struct sockaddr *)&from,&fromlen);
        ev_reg_num=atoi(buf);
        printf("\nN_reg from client %d\n", ev_reg_num); //[DEBUG]
        FILE *outfile;
        outfile = fopen(reg_file[ev_reg_num],"a"); // Will append to current file
        //char user[]="Brian Johnson"; //Temporary username
        mytime = time(NULL);
        fprintf(outfile,"User %s is going. %s",username,ctime(&mytime)); //Write to file
        fclose(outfile); //Close wirite file
        printf("\n Client %s has Registered",username);
      }

      if(strcmp(buf,"n_teste")==0){
        printf("\n Testing:\n");
        length=sizeof(struct sockaddr_in);
        n = sendto(sock,"ping",4,0,(struct sockaddr *)&from,fromlen);
        if (n < 0) error("Error sending ping to Client");
      }
      /*
      int i;
      i=0;
      i=atoi(buf);
      if(i==1)printf(" It's me One :D \n" );
      //event.name[i]
      printf("S:Number:%d!\n",i );
      */

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

      //n = sendto(sock,"S:Got your message\n",19,0,(struct sockaddr *)&from,fromlen);
      //if (n < 0) error("Error in sendto");
  }//while
}

//==== ERROR ====//
void error(char *msg) { // error messages: print message and terminate program
	perror(msg);
	exit(1);
}
//===============//
