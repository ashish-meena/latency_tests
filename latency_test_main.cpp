#include "utils.h"
#include "icmp_ping.h"
#include"tcp_client.h"
#include"udp_client.h"

int main(int argc, char**argv)
{

    string targetHost1 = "127.0.0.1";
    string targetHost2 = "192.168.0.1";
    if (argc == 2)
    {
        targetHost2=argv[1];
    }
    int num_packets = 10;
    int sleep_time = 0;
    int max, min,median,average;
    vector<int> stats;
    int tcp_blocking_port = 51234;
    bool is_blocking = false;
    string send_message = "ping";
    cout <<"Pinging " << targetHost1 << " using icmp, num_packets="<< num_packets<<
         ",sleep_time=" << sleep_time<< endl;
    stats = ping_server_collect_stats(targetHost1, num_packets);
    get_min_max_median(stats, min, max, median, average);
    cout <<"Stats were PacketsReceived="<<stats.size() <<",MinTime=" <<min<< " MaxTime="<<max <<"MedianTime="<<median<<"AverageTime="<< average <<endl;

    cout <<"Pinging google.com using icmp, num_packets="<< num_packets<<
         ",sleep_time=" << sleep_time<< endl;
    stats = ping_server_collect_stats("www.google.com", num_packets);
    get_min_max_median(stats, min, max, median, average);
    cout <<"Stats were PacketsReceived="<<stats.size() <<",MinTime=" <<min<< " MaxTime="<<max <<"MedianTime="<<median<<"AverageTime="<< average <<endl;

    cout <<"Pinging " << targetHost2 << " using icmp, num_packets="<< num_packets<<
         ",sleep_time=" << sleep_time<< endl;
    stats = ping_server_collect_stats(targetHost2.c_str(), num_packets);
    get_min_max_median(stats, min, max, median, average);
    cout <<"Stats were PacketsReceived="<<stats.size() <<",MinTime=" <<min<< " MaxTime="<<max <<"MedianTime="<<median<<"AverageTime="<< average <<endl;


    cout <<"Pinging " << targetHost1 << " on port " << tcp_blocking_port << "num_packets="<< num_packets<<
         ",sleep_time=" << sleep_time<< "is_non_blocking=false"<< endl;
    stats = ping_tcp_server_collect_stats(targetHost1.c_str(), tcp_blocking_port, sleep_time,num_packets, "ping",false);

    get_min_max_median(stats, min, max, median, average);
    cout <<"Stats were PacketsReceived="<<stats.size() <<",MinTime=" <<min<< " MaxTime="<<max <<"MedianTime="<<median<<"AverageTime="<< average <<endl;

    cout <<"Pinging " << targetHost1 << " on UDP port " << udp_blocking_port << "num_packets="<< num_packets<<
         ",sleep_time=" << sleep_time<< "is_non_blocking=false"<< endl;

    vector<int> stats=ping_udp_server_collect_stats(targetHost1, tcp_blocking_port, sleep_time,num_packets, send_message, false);
}
