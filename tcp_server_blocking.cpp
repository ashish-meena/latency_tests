/*
 * tcpserver.c - TCP echo server
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
#include<pthread.h>
#include<vector>
using namespace std;
#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
void error(const char *msg) {
    perror(msg);
    exit(1);
}
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *client_fd1)
{
    /*
     * read: read input string from the client
     */
    int n;
    char buf[BUFSIZE]; /* message buffer */
    bzero(buf, BUFSIZE);
    int client_fd = *(int *) client_fd1;
    while ((n = read(client_fd, buf, BUFSIZE))>0)
    {
        printf("server received %d bytes: %s\n", n, buf);

        /*
         * write: echo the input string back to the client
         */
        n = write(client_fd, buf, strlen(buf));

        if (n < 0)
            error("ERROR writing to socket");

    }
    close(client_fd);
}

int main(int argc, char **argv) {
    int server_fd; /* parent socket */
    int client_fd; /* child socket */
    int portno; /* port to listen on */
    int clientlen; /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval; /* flag value for setsockopt */
    vector<pthread_t> threads; /*stores all threads */

    /*
     * check command line arguments
     */
    if (argc == 2) {
        portno = atoi(argv[1]);
    }
    else {
        portno = 51234;
        cout << "Using default port " << portno << endl;
    }
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        error("ERROR opening socket");

    optval = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR,
               (const void *)&optval , sizeof(int));

    bzero((char *) &serveraddr, sizeof(serveraddr));

    serveraddr.sin_family = AF_INET;

    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

    serveraddr.sin_port = htons((unsigned short)portno);

    if (bind(server_fd, (struct sockaddr *) &serveraddr,
             sizeof(serveraddr)) < 0)
        error("ERROR on binding");

    if (listen(server_fd, 32) < 0)
        error("ERROR on listen");

    clientlen = sizeof(clientaddr);
    while (1) {

        pthread_t thread_id;
        client_fd = accept(server_fd, (struct sockaddr *) &clientaddr, (socklen_t *)&clientlen);
        if (client_fd < 0)
            error("ERROR on accept");
        if( pthread_create( &thread_id , NULL , connection_handler , (void*) &client_fd) < 0)
        {
            perror("could not create thread");
            return 1;
        }
        threads.push_back(thread_id);
    }

    /*As of now below code is redundant and unreachable but if we change while loop to work on a condition, we need to ensure that we dont terminate while threads still running */
    for (int i = 0 ; i < threads.size(); i++)
    {
        pthread_join (threads[i], NULL);
    }

}

