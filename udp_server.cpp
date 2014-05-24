/*
UDP Echo Server
*/
#include "utils.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include<iostream>
using namespace std;
#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
void error(const char *msg) {
    perror(msg);
    exit(1);
}
int main(int argc, char **argv) {
    int parentfd; /* parent socket */
    int childfd; /* child socket */
    int portno; /* port to listen on */
    int clientlen; /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    struct hostent *hostp; /* client host info */
    char buf[BUFSIZE]; /* message buffer */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval; /* flag value for setsockopt */
    int n; /* message byte size */
    char is_blocking_str[5];
    bool is_blocking = false;
    /*
     * check command line arguments
     */
    if (argc == 3) {
        portno = atoi(argv[1]);
        strcpy(is_blocking_str, argv[2]);

        if (strcmp(is_blocking_str,"true")==0)
        {
            is_blocking = true;
        }
    }
    else {
        portno = 45123;
        cout <<" using default port " << portno;
    }

    /*
     * socket: create the parent socket
     */
    parentfd =socket(AF_INET, SOCK_DGRAM, 0);

    if (parentfd < 0)
        error("ERROR opening socket");


    optval = 1;
    setsockopt(parentfd, SOL_SOCKET, SO_REUSEADDR,
               (const void *)&optval , sizeof(int));

    if (is_blocking)
    {
        cout <<"Setting it to non blocking " << endl;
        if (!set_socket_blocking_socket(parentfd, true))
        {
            error("Error trying to set to non blocking");
        }
    }
    /*
     * build the server's Internet address
     */
    bzero((char *) &serveraddr, sizeof(serveraddr));

    /* this is an Internet address */
    serveraddr.sin_family = AF_INET;

    /* let the system figure out our IP address */
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* this is the port we will listen on */
    serveraddr.sin_port = htons((unsigned short)portno);

    /*
     * bind: associate the parent socket with a port
     */
    if (bind(parentfd, (struct sockaddr *) &serveraddr,
             sizeof(serveraddr)) < 0)
        error("ERROR on binding");

    /*
     * main loop: wait for a connection request, echo input line,
     * then close connection.
     */
    clientlen = sizeof(clientaddr);
    int recv_len;
    while (1) {
        //printf("Waiting for data...");
        //     fflush(stdout);

        bzero(buf, BUFSIZE);
        //try to receive some data, this is a blocking call
        if ((recv_len = recvfrom(parentfd, buf, BUFSIZE, 0, (struct sockaddr *) &clientaddr, (socklen_t *)&clientlen))<0)
        {
            error("recvfrom()");
        }
        //now reply the client with the same data
        if (sendto(parentfd, buf, recv_len, 0, (struct sockaddr*) &clientaddr, clientlen) == -1)
        {
            error("sendto()");
        }

    }
}
