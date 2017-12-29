/* Creates a datagram server (uses UDP).  
   The port number is passed as an argument.  
   This server runs forever */
// changed by Paulo Coimbra, 08.10.27, 2014-10-25
//---------------------------------------------------------
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
//---------------------------------------------------------
// trata mensagens de erro
void error(char *msg) {
    perror(msg);
    exit(0);
}
//---------------------------------------------------------
int main(int argc, char *argv[]) {

   int sock, length, fromlen, n;
   struct sockaddr_in server;
   struct sockaddr_in from;
   char buf[1024];
   char text_msg[256];// message (text) to send

   if (argc < 2) {
      fprintf(stderr, "ERROR, no port provided\n");
      exit(0);
   }

   // cria socket datagram (UDP)
   sock=socket(AF_INET, SOCK_DGRAM, 0);
   if (sock < 0) error("Error in opening socket");
   length = sizeof(server);
   bzero(&server,length);
   server.sin_family=AF_INET;
   server.sin_addr.s_addr=INADDR_ANY;
   server.sin_port=htons(atoi(argv[1]));
   if (bind(sock,(struct sockaddr *)&server,length)<0) 
       error("Error in binding");
   fromlen = sizeof(struct sockaddr_in);

   //forever cicle for clients
   while (1) {
       write(1,"Waiting for a client...",23);

       //receives datagrama that identifies client
       n = recvfrom(sock,buf,1024,0,(struct sockaddr *)&from,&fromlen);
       if (n < 0) error("Error in recvfrom");
       write(1,"Received a datagram: ",21);
       write(1,buf,n);
       //sends response text
       n = sendto(sock,"Got your message\n",17,
                  0,(struct sockaddr *)&from,fromlen);
       if (n  < 0) error("Error in sendto");
   }//while
 }
//---------------------------------------------------------
//---------------------------------------------------------


