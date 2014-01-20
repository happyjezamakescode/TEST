#include <stdio.h>
#include <sys/types.h> 
#include <sys/ioctl.h> 
#include <signal.h>
#include "socklib.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h> 

#define FSOCKBUFSIZ 20480

int sockfd, newsockfd;
SOCKET *fSocket=NULL;

//instead of server_bind and server_listen we could use 
//server_connect
int server_connect(int portno)
{

    /* Create client socket */ 
    if (!fSocket) {
      fSocket = sopen();
    }

    printf ("socket fSocket %p\n", fSocket);
    if ((sockfd = sserver(fSocket, portno, S_DELAY)) >= 0) {
//      fConnected = 1;
      return 0;   //good
    }

    if (fSocket) sclose(fSocket);

    return -1;  //not good
}

int server_readline(char *buffer)
{

  int nBytesRead = sreadline(sockfd, buffer, strlen(buffer));

//  if (nBytesRead <= 0) {
//    fConnected = 0;
//  }

  return nBytesRead;

}
 
int server_bind(int portno)
{
	struct sockaddr_in serv_addr;

	sockfd = socket(AF_INET, SOCK_STREAM & SO_KEEPALIVE, 0);
	if (sockfd < 0) {
	        puts("ERROR opening socket"); return -1;}
     	bzero((char *) &serv_addr, sizeof(serv_addr));
     	serv_addr.sin_family = AF_INET;
     	serv_addr.sin_addr.s_addr = INADDR_ANY;
     	serv_addr.sin_port = htons(portno);
     	if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) {
              puts("ERROR on binding"); return -1;}

     	return 0;
}

int server_listen()
{
  struct sockaddr_in cli_addr;
  int clilen;
  int yes=1;

  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  newsockfd = accept(sockfd, 
      (struct sockaddr *) &cli_addr, 
      (socklen_t *) &clilen);

  if (newsockfd < 0) {
    puts("ERROR on accept"); 
    return -1;
  }

  ioctl(newsockfd,FIOASYNC,&yes);
  //       	ioctl((int )stdin,FIOASYNC,&yes);
}

int server_close()
{
	close(sockfd);
	close(newsockfd);
}

int server_read(char *buffer)
{
  int n;
  bzero(buffer,FSOCKBUFSIZ);
  n = read(newsockfd,buffer,FSOCKBUFSIZ-1);
//  n = read(sockfd,buffer,16659);

  return n;
}

int server_write(char *buffer)
{
	int n;
       	n = write(newsockfd,buffer,strlen(buffer));
       	return n;
}


int nsock2fd[10];
int sock2fd;
struct hostent *server;
int clientID=1;

int client_select( int clientno)
{
//	if(nsock2fd[clientno]!=NULL) {
	  clientID = clientno;
	  sock2fd = nsock2fd[clientno];
//	}
//	DM, 31.08.2009: there is no return value, how does it work??
//	DM answer:
//	DM, 31.08.2009: why do we set sock2fd to something here?
//	DM answer: in order to use client_write function
}

int client_connect(char *hostname, int portno)
{
  int flags;
	int yes=1;
	struct sockaddr_in serv_addr;
  //TEST
  struct linger fix_ling;
  int optval=1;
  socklen_t optlen=4;
  //ETEST

	sock2fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock2fd < 0) {
        	puts("ERROR opening socket"); return -1;}

	server = gethostbyname(hostname);
	if (server == NULL) {
		puts("ERROR, no such host"); printf("%s\n",hostname); return -1;}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
		(char *)&serv_addr.sin_addr.s_addr,
		server->h_length);
	serv_addr.sin_port = htons(portno);

//TEST
  fix_ling.l_onoff = 1;
  fix_ling.l_linger = 0; 
  if (setsockopt(sock2fd, SOL_SOCKET, SO_LINGER, &fix_ling, sizeof(fix_ling))==-1) {
    perror("sopen(setsockopt)");
  }
  if (setsockopt(sock2fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, optlen)==-1) {
    perror("sopen(setsockopt)");
  }
  //ETEST
  
	if (connect(sock2fd,(const struct sockaddr *)&serv_addr,
		    sizeof(serv_addr)) < 0) {
	  //	puts("ERROR connecting"); 
	  return -1;
	}

	//	fcntl(sock2fd, F_SETOWN, getpid());
	//	fcntl(sock2fd, F_SETFL, FASYNC);

	//	flags = fcntl(sock2fd, F_GETFL, 0);
	//	fcntl(sock2fd, F_SETFL, flags | O_NONBLOCK);

	nsock2fd[clientID] = sock2fd;
	return 0;
}

int client_read(char *buffer)
{
	int n;
       	bzero(buffer,6600);
       	n = read(sock2fd,buffer,6659);
       	return n;
}

int client_write(char *buffer)
{
        printf("1, buffer length is %d of socket %d\n",strlen(buffer),sock2fd);
	int n;
        printf(" writing 1 ...");
//       	n = write(sock2fd,buffer,strlen(buffer));
        n = send(sock2fd, buffer,strlen(buffer),MSG_DONTWAIT);
        printf("written %d bytes \n",n);
       	return n;
}

int client_write2(char *buffer, int clientno)
{
        printf("2, buffer length is %d of socket %d\n",strlen(buffer),nsock2fd[clientno]);
	int n;
        printf(" writing 2 ...");
//       	n = write(sock2fd,buffer,strlen(buffer));
	      clientID = clientno;
        n = send(nsock2fd[clientno], buffer,strlen(buffer),MSG_DONTWAIT);
        printf("written %d bytes \n",n);
       	return n;
}

int client_close()
{
	close(sock2fd);
}

int client_close2(int clientno)
{
	clientID = clientno;
	close(nsock2fd[clientno]);
}


