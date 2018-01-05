#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h> // strcat
#include <string.h> // str
//==== DEFAULT SOCKETS ====//
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
//========================//
#define MAX_CAR 100
#define MAX_EVENT_NUM 20

// NOTE CHANGE system("cls") to system("clear") for linux
int iexit;
char c_exit;
int menu_choice;

//==== Menu READ VAR ====//
char server_ip[]="192.168.1.254";
int current_port=5000;
//======================//

char read_file_name[]="sauce/Events_List.txt";   // File to read
char write_file_nameE[]="cloudpoint3exit_qsortE.txt";

struct Events{
  char name[MAX_EVENT_NUM][MAX_CAR];
  int num_event;// Number of Events in List
};


int main(int argc, char *argv[]){
  //==== Code Mesh ====//
  int sockfd, portno, n;  //socket file descriptor, port number
  struct sockaddr_in serv_addr;  //server address data
  struct hostent *server;
  char buffer[256];  // data (bytes) to be sent to server
  //===================//
  struct Events event;
  event.num_event=0;

  iexit=0; // Forces Enter Menu
  menu_choice=6;
  char yes;

  //==== Menu SET VAR ====//
  int port_num; // Port Number
  int ev_reg_num;
  //======================//

  //==== Valid Input's Check ====//
  if (argc < 3) {
     fprintf(stderr,"usage %s hostname port\n", argv[0]);
     exit(0);
  }
  //=============================//

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

		//editStud();
	    break;

	    case 2:
		system("clear");
		printf("==== Set_Ports ====\n");
		port_num =0;
		while(port_num<49152 || port_num>65535){
		    printf(" Set Port(49152-65535): ");
		    scanf("%d",&port_num);
		    current_port=port_num;

		}
		//delStud();
	    break;

	    case 3:
		system("clear");
		printf("==== Get_Event_List ====\n");
		printf("\nThere are %d Events Up. Show Event List?(y/n):",event.num_event);
		scanf("%s",&yes);
		if(strcmp(&yes,"y")==0){ // Display events
		    int i;
		    for(i=1;i<event.num_event+1;i++){
			printf("No%d, Event: %s",i,event.name[i]);
		    }
		}
		printf("\nReturn to Menu(y/n):");
		scanf("%s",&yes); // DEBUG
	    break;

	    case 4:
		system("clear");
		printf("==== Make_Registration ====\n");
		ev_reg_num=0;
		printf("Whats the Event Number you whould like to enter(0-%d)",event.num_event);
		scanf("%d",&ev_reg_num); // Input event number



	    break;

	    case 5:
		system("clear");

	    break;
	    menu_choice=6; // Reset Menu Coice
	}
    }
    //=============//
    exit(0);
    return 0;
}
