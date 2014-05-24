#ifndef ICMP_PING_H
#define ICMP_PING_H
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/time.h>

#include <netinet/in_systm.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include<vector>

using namespace std;
uint16_t in_cksum(uint16_t *addr, unsigned len);

struct sockaddr_in create_raw_socket(string target, int &sock_fd);
vector<int> ping_server_collect_stats(string target, int num_packets);
int ping(sockaddr_in to, int socked_fd,int seq_num, long long &time_taken);

#endif
