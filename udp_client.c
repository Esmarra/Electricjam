/*=========================================
Program Name :	 udp_client
Base Language:	 English
Created by   :	 Esmarra
Creation Date:	 09/01/2018
Rework date entries:
* 10/01/2018
Program Objectives:
Observations:
* Based on: client_udp by Paulo Coimbra, 2014-10-25
* Git Repo: https://github.com/Esmarra/Electricjam
Special Thanks:
=========================================*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h> //strcat
#include <string.h> //strcmp
//==== DEFAULT SOCKETS ====//
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //Read ip ( inet_ntoa )
#include <netdb.h>
//========================//
#define MAX_CAR 100 //Max strlen event name can have (REMOVE FOR CLIENT?)
#define MAX_EVENT_NUM 20 //Max Number of Events
#define MAX_USERNAME 20 //Max size of username

int iexit; //Keep user in menu (var)
char c_exit; //Exit Menu aka Program
int menu_choice; //Menu choice (var)
char username[MAX_USERNAME]; //Username

//==== Menu READ VAR ====//
char server_ip[12]=""; //Selected Server Ip
int current_port; //Current Used Port
//======================//

struct Events{ //Stores Events in struct (Used on Server & Client Side)
  char name[MAX_EVENT_NUM][MAX_CAR]; //Stores Event name
  int num_event;//Number of Events in List
};

struct Users{ //Stores Users in struct (Used on Server & Client Side)
  char username[MAX_EVENT_NUM][MAX_CAR];//Max users is 20
  int regist_arr[MAX_EVENT_NUM];
  int user_num;//Total Users
};

//==== Functions ====//
void error(char *); // Error (funct)
//===================//

int main(int argc, char *argv[]){
  //==== Code Mesh ====//
  int sock, length, n;
  struct sockaddr_in server, from; //Haddle Internet Addresses
  struct hostent *hp; //Represent Hosts Entry
  char buffer[256]; //Data (bytes) to be sent to server
  //===================//

  struct Events event; //Start struct
	event.num_event=0; //Init zeros
  struct Users user; //Start Users struct
  user.user_num=0; //Init zero
  iexit=0; //Forces Enter Menu
  menu_choice=6;
  char yes;
  strcpy(server_ip, argv[1]); //Get Server ip from terminal (Right Practice)
  current_port=atoi(argv[2]); //Port is given via terminal untill changed
  int reg_bool=0; //See if user registred this session
  int regist_arr[MAX_EVENT_NUM];
  int reg_count=0;
  int reg_array=0;// Nuber of registrations the user has from server
  //==== Menu SET VAR ====//
  int port_num; //Port Number
  int ev_reg_num; //Register Event (var)
  //======================//

  if (argc < 3) { //Valid Input's Check
     fprintf(stderr,"usage %s hostname port\n", argv[0]);
     exit(0);
  }

  system("clear");
  while(iexit!=1){
    system("clear"); //Clear Window
    //printf("INPUT:%s\n",server_ip); [DEBUG]
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
  	scanf("%d",&menu_choice); //Read Input

  	switch(menu_choice){
      case 0:
      system("clear");
  		printf(" Are you sure you want to exit?(y/n): ");
  		scanf("%s",&c_exit);
  		if(c_exit=='y'){ //Confirm_exit
  		    printf(" Terminating Program...\n");
  		    iexit=1; //Exits Menu, and ends Program
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
  		while(port_num<49152 || port_num>65535){ //Valid check
  		    printf(" Set Port(49152-65535): "); //Avalible Server Ports?
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
      //printf("use_ip=%s!\n",server_ip ); //[DEBUG]
      hp = gethostbyname(server_ip); //192.168.1.84
      if (hp==0) error("Unknown host");
      bcopy((char *)hp->h_addr,(char *)&server.sin_addr,hp->h_length);
      server.sin_port = htons(current_port);
      length=sizeof(struct sockaddr_in);
      //==============================//
      n=sendto(sock,"n_event",7,0,&server,length); //Get number of events avalible
      if (n < 0) error("Error sending n_events to Server");

      bzero(buffer,256);
      n = recvfrom(sock,buffer,256,0,&from, &length);//Gets event number from server
      if (n < 0) error("Error getting n_events from Server");
      event.num_event=atoi(buffer); // Convert char to int
      //printf("\nN_evnt from server %d\n", event.num_event); //[DEBUG]
      printf("Please Type Username:");
      scanf("%s",&user.username[0]);//Store in ZERO

      length=sizeof(struct sockaddr_in); // Fixes Error(no idea why tho)
      bzero(buffer,256);
      n=sendto(sock,"n_usere",7,0,&server,length); //Tells Server User to start user
      if (n < 0) error("Error sending n_usere to Server");

      length=sizeof(struct sockaddr_in); // Fixes Error(no idea why tho)
      bzero(buffer,256);
      n=sendto(sock,user.username[0],strlen(user.username[0]),0,&server,length); //Send Username[0] aka this client
      if (n < 0) error("Error sending Username to Server");
      //Server Sends user_num, Client syncs with server (For better Registration)
      bzero(buffer,256);
      n = recvfrom(sock,buffer,256,0,&from, &length);//Gets user_num from server
      if (n < 0) error("Error getting user_num from Server");
      user.user_num=atoi(buffer);
      printf(" User_num(form server):%d\n",user.user_num); //[DEBUG]
      strcpy(user.username[user.user_num],user.username[0]);//Copy URS[0] to the user thats in server


  		printf(" There are %d Events Up. Show Event List?(y/n):",event.num_event);
  		scanf("%s",&yes);
  		if(strcmp(&yes,"y")==0){ // Display events
        //==== Send d_event (flag) to server ====
        length=sizeof(struct sockaddr_in); // Fixes Error(no idea why tho)
        bzero(buffer,256);
        n=sendto(sock,"d_event",7,0,&server,length);
        if (n < 0) error("Error sending d_event to Server");
        //==== Get events from server ====//
        int i=0;
        for(i=1;i<event.num_event+1;i++){//Read buffer n_evt times
          bzero(buffer,256); //Reset every time
          n = recvfrom(sock,buffer,256,0,&from, &length); //Recive Events
          if (n < 0) error("Error getting Events");
          printf("  No %d,Event: %s",i,buffer); //Print Events
          strcpy(event.name[i], buffer); //Copy Values to Structure
        }
        //================================//
  		}
      /*
      printf("Reading struct\n");
      int i;
      for(i=1;i<event.num_event+1;i++){
        printf("Ev %d Name: %s",i,event.name[i-1]);
      }
      */
  		printf("\n Return to Menu(y/n):");
  		scanf("%s",&yes); //Exit Case
      break;

      case 4:
  		system("clear");
  		printf("==== Make_Registration ====\n");
  		ev_reg_num=0;
      length=sizeof(struct sockaddr_in); //Fixes Error(no idea why tho)
      bzero(buffer,256);
      n=sendto(sock,"n_regis",7,0,&server,length);
      if (n < 0) error("Error sending n_regis to Server");

      while(ev_reg_num<1 || port_num>event.num_event){ //Force Valid Input
  		    printf(" Whats the Event Number you whould like to enter(1-%d):",event.num_event);
  		      scanf("%d",&ev_reg_num); //Input event number
      }

      char temp[]=""; //Char to send Int (Sends event to register)
      sprintf(temp,"%d",ev_reg_num); //Int to str
      n = sendto(sock,temp,strlen(temp),0,&server,length);//Send to server what event to register
      if (n < 0) error("Error sending ev_reg_num");

      char tem[]=""; //Create a temp Char (Sends user_num)
      sprintf(tem,"%d",user.user_num); //Convert (int) to (char)
      n = sendto(sock,tem,strlen(tem),0,&server,length);//Send to server what event to register
      if (n < 0) error("Error sending user_num");
      //ASK USER FOR NUMBER OF SEATS???
      //Do Server Validation Here
      bzero(buffer,256); //Reset every time
      n = recvfrom(sock,buffer,256,0,&from, &length); //Recive Reg Validation
      if (n < 0) error("Error geting Validation");

      if(strcmp(buffer,"not_reg")==0){
        printf(" Resgistration Not Valid");
      }else{
        printf(" Registred on %s\n",event.name[ev_reg_num]);
        reg_bool=1;
        regist_arr[reg_count]=ev_reg_num; //Stores num env registred to array
        reg_count++;
      }
      printf("\n Return to Menu(y/n):");
  		scanf("%s",&yes); //Exit Case
      break;

      case 5:// Show all user registration (get from server)
      system("clear");
      printf("==== View_Registrations ====\n");
      length=sizeof(struct sockaddr_in); //Fixes Error(no idea why tho)
      n=sendto(sock,"get_reg",7,0,&server,length);
      if (n < 0) error("Error sending get_reg to Server");

      char temm[]=""; //Create a temp Char (Sends user_num)
      sprintf(temm,"%d",user.user_num); //Convert (int) to (char)
      n = sendto(sock,temm,strlen(temm),0,&server,length);//Send to server what event to register
      if (n < 0) error("Error sending user_num");

      bzero(buffer,256); //Reset every time
      n = recvfrom(sock,buffer,256,0,&from, &length); //Recive reg_array size
      if (n < 0) error("Error geting reg_array num");
      reg_array=atoi(buffer);
      printf(" Reg_array %d\n",reg_array);
      int i;
      for(i=0;i<reg_array;i++){//Read buffer reg_array times
        bzero(buffer,256); //Reset every time
        n = recvfrom(sock,buffer,256,0,&from, &length); //Recive Registred Events
        if (n < 0) error("Error getting Registred Events");
        printf("  Registred: %s",buffer); //Print Events
      }
      /*
      if(reg_bool==1){// If there was a registration current session(not from server)
        int i;
        for(i=0;i<reg_count;i++){
          printf("\n  You have sucesefully registred in %s",event.name[regist_arr[i]]); //Print all registrations
        }
      }
      */
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
