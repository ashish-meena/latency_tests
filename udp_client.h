#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include<arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include<strings.h>
#include<cstring>
#include <iostream>
#include <cstdlib>
#include<vector>
#include<string>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

struct sockaddr_in create_udp_socket(const char* target, int port, bool is_non_blocking, int &sockfd);
vector<int> ping_udp_server_collect_stats(const char* target, int port, int sleep,int num_packets, const char *send_message, bool is_non_blocking);
int udp_ping(struct sockaddr_in &servaddr, int sockfd, long long &time_taken,const char* send_message);

#endif
