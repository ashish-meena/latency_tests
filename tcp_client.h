#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include<arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include<strings.h>
#include<cstring>
#include <iostream>
#include <cstdlib>
#include <vector>
#include<string>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

vector<int> ping_tcp_server_collect_stats(const char* target, int port, int sleep,int num_packets, const char *send_message, bool is_non_blocking);
int tcp_ping(int sockfd, long long &time_taken,const char* send_message);

int connect_to_server(const char* target, int port);


#endif
