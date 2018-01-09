/*=========================================
Program Name :	 udp_client
Base Language:	 Mixed English
Created by   :	 Esmarra
Creation Date:	 09/01/2018
Rework date entries:
Program Objectives:
Observations:
* Based on: client_udp by Paulo Coimbra, 2014-10-25
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
#define MAX_CAR 100 // Max strlen event name can have (REMOVE FOR CLIENT?)
#define MAX_EVENT_NUM 20 // Max Number of Events

int iexit; // Keep user in menu (var)
char c_exit; // Exit Menu aka Program
int menu_choice; // Menu choice (var)

//==== Menu READ VAR ====//
char server_ip[]=""; // Comeback here later on <--------------->
int current_port=50000; // Comeback here later on <--------------->
//======================//

struct Events{ // Stores Events in struct (Used on Server & Client Side)
  char name[MAX_EVENT_NUM][MAX_CAR]; // Stores Event name
  int num_event;// Number of Events in List
};

//==== Functions ====//
void error(char *); // Error (funct)
//===================//

int main(int argc, char *argv[]){
  //==== Code Mesh ====//
  int sock, length, n;
  struct sockaddr_in server, from;
  struct hostent *hp; // SHOW HOSTNAME (NOT WORKING???)
  char buffer[256]; // data (bytes) to be sent to server
  //===================//

  struct Events event; // Start struct
	event.num_event=0; // Init zeros
  iexit=0; // Forces Enter Menu
  menu_choice=6;
  char yes;
  strcpy(server_ip, argv[1]); // Right Practice NOT WORKING <--------------->
  current_port=atoi(argv[2]); // NOT WORKING <--------------->

  //==== Menu SET VAR ====//
  int port_num; // Port Number (REMOVE??) NO
  int ev_reg_num; // Event NUM (REMOVE??) NO
  //======================//

  if (argc < 3) { // Valid Input's Check
     fprintf(stderr,"usage %s hostname port\n", argv[0]);
     exit(0);
  }

  system("clear");
  while(iexit!=1){
    system("clear"); //Clear Window

    //==== Head Display ====//
    printf("\n -----------------------------------------------\n");
    printf("  SetEvent Client 2017\n");
    printf("  by Miguel Maranha & Vitaliy Rudenko (DEEC-UC)\n");
    printf("  Current event server: %s\n",server_ip);
    printf("  Current port: %d",current_port);
    printf("\n -----------------------------------------------\n");
    //======================//

    //===== Menu ====//
    printf(" Menu:\n");
    printf("  0: Exit\n");
    printf("  1: Set event server\n");
    printf("  2: Set port (49152-65535)\n");
    printf("  3: Get list of events\n");
    printf("  4: Make registration\n");
    printf("  5: Show registration list [extra]\n");
    //===============//

    printf(" Choice: ");
  	scanf("%d",&menu_choice); // Read Input

  	switch(menu_choice){
      case 0:
      system("clear");
  		printf(" Are you sure you want to exit?(y/n): ");
  		scanf("%s",&c_exit);
  		if(c_exit=='y'){ // Confirm_exit
  		    printf(" Terminating Program...\n");
  		    iexit=1; // Exits Menu, and ends Program
  		}
      break;

      case 1:
      system("clear");
  		printf("==== Client_Server ====\n");
  		printf(" Set destination IP adress: ");
  		scanf("%s",server_ip);
      printf(" New Server is:%s!",server_ip);
      break;

      case 2:
      system("clear");
  		printf("==== Set_Ports ====\n");
  		port_num=0;
  		while(port_num<49152 || port_num>65535){
  		    printf(" Set Port(49152-65535): "); // Avalible Server Ports?
  		    scanf("%d",&port_num);
  		    current_port=port_num;
  		}
      break;

      case 3:
  		system("clear");
  		printf("==== Get_Event_List ====\n");
      //==== Start UDP Connection ====//
      sock= socket(AF_INET, SOCK_DGRAM, 0);
      if (sock < 0) error("socket");
      server.sin_family = AF_INET;
      hp = gethostbyname(argv[1]);
      if (hp==0) error("Unknown host");
      bcopy((char *)hp->h_addr,(char *)&server.sin_addr,hp->h_length);
      server.sin_port = htons(atoi(argv[2]));
      length=sizeof(struct sockaddr_in);
      //==============================//
      n=sendto(sock,"n_event",7,0,&server,length); // Get number of events avalible
      if (n < 0) error("Error sending n_events to Server");

      bzero(buffer,256);
      n = recvfrom(sock,buffer,256,0,&from, &length);// gets event num from server
      if (n < 0) error("Error getting n_events from Server");
      event.num_event=atoi(buffer); // Convert char to int
      //printf("\nN_evnt from server %d\n", event.num_event); //[DEBUG]

  		printf(" There are %d Events Up. Show Event List?(y/n):",event.num_event);
  		scanf("%s",&yes);
  		if(strcmp(&yes,"y")==0){ // Display events
        //Send d_event to server
        length=sizeof(struct sockaddr_in); // Fixes Error(no idea why tho)
        bzero(buffer,256);
        n=sendto(sock,"d_event",7,0,&server,length);
        if (n < 0) error("Error sending d_event to Server");
        //Get events from server
        // do this for the number of events?? otherwise buffer will be full??
        int i=0;
        for(i=1;i<event.num_event+1;i++){//Read buffer n_evt times
          bzero(buffer,256); //Reset every time
          n = recvfrom(sock,buffer,256,0,&from, &length); //Recive Events
          if (n < 0) error("Error getting Events");
          printf("  No %d,Event: %s",i,buffer); //Print Events
          strcpy(event.name[i-1], buffer); //Copy Values to Structure
        }
  		}

      printf("Reading struct\n");
      int i;
      for(i=1;i<event.num_event+1;i++){
        printf("Ev %d Name: %s",i,event.name[i-1]);
      }
  		printf("\n Return to Menu(y/n):");
  		scanf("%s",&yes); //Exit Case
      break;

      case 4:
  		system("clear");
  		printf("==== Make_Registration ====\n");
  		ev_reg_num=0;
      length=sizeof(struct sockaddr_in); // Fixes Error(no idea why tho)
      bzero(buffer,256);
      n=sendto(sock,"n_regis",7,0,&server,length);
      if (n < 0) error("Error sending n_regis to Server");

  		printf("Whats the Event Number you whould like to enter(0-%d):",event.num_event);
  		scanf("%d",&ev_reg_num); // Input event number
      char temp[]=""; // Char to send Int
      sprintf(temp,"%d",ev_reg_num); // Int to str
      n = sendto(sock,temp,strlen(temp),0,&server,length);
      if (n < 0) error("Error sending ev_reg_num");
      printf("Sucesefully registred on %s\n",event.name[ev_reg_num]);



      printf("\n Return to Menu(y/n):");
  		scanf("%s",&yes); //Exit Case
      break;

      case 5:// Show all user registration or registrations done on this session?
      system("clear");
      printf("==== View_Registrations_Current_Session ====\n");

      printf("\n Return to Menu(y/n):");
  		scanf("%s",&yes); //Exit Case
      break;
      menu_choice=6; // Reset Menu Coice
  	}
  }
  //=============//
  exit(0);
  return 0;
}

//==== ERROR ====//
void error(char *msg) {
    perror(msg);
    exit(0);
}
//===============//

//==== Might Be Usefull Later On ====//
/*
length=sizeof(struct sockaddr_in); // Fixes Error(no idea why tho)
bzero(buffer,256);
n=sendto(sock,"n_teste",7,0,&server,length);
if (n < 0) error("Error sending test to Server");

bzero(buffer,256); //Reset
n = recvfrom(sock,buffer,256,0,&from, &length); //Revice Events
if (n < 0) error("Error getting Events");
printf("S:%s",buffer); // Print
*/
//==========================//
