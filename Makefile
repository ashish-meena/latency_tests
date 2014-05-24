all:
tcp_server_blocking tcp_server_non_blocking udp_server latency_main
#all: icmp_ping tcp_client udp_client tcp_server_blocking tcp_server_non_blocking udp_server latency_main
latency_main:
g++ -o latency_main icmp_ping.h icmp_ping.cpp tcp_client.h tcp_client.cpp udp_client.h udp_client.cpp utils.h utils.cpp latency_test_main.cpp
#icm_ping.o:
#	g++ icmp_ping.cpp

#tcp_client:
#	g++ -o tcp_client utils.h tcp_client.h tcp_client.cpp

#udp_client:
#	g++ -o udp_client utils.h udp_client.cpp

tcp_server_blocking:
g++ -o tcp_server_blocking -pthread utils.h tcp_server_blocking.cpp

tcp_server_non_blocking:
g++ -o tcp_server_non_blocking utils.h utils.cpp tcp_server_non_blocking.cpp

udp_server:
g++ -o udp_server utils.h utils.cpp udp_server.cpp
clean:
rm -rf tcp_client udp_client tcp_server_blocking tcp_server_non_blocking udp_server udp_client icmp_ping latency_main *gch
