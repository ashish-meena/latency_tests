#include "utils.h"
#include"tcp_client.h"
#define SEC2NANO 1000000000.0f

using namespace std;

vector<int> ping_tcp_server_collect_stats(const char* target, int port, int sleep,int num_packets, const char *send_message, bool is_non_blocking)
{
    vector<int> stats;
    int serverfd = connect_to_server(target, port);
    if (is_non_blocking)
    {
        set_socket_blocking_socket(serverfd, true);
    }
    for (int i = 0; i < num_packets; i++)
    {
        //putting a very long time as default for now
        long long tmp = 10000000000000000;
        char send_message_tmp[1024];
        string send_string(send_message);

        //appending i to ping to ensure uniqueness of message
        std::stringstream sstm;
        sstm << send_string << i;
        send_string = sstm.str();

        strcpy (send_message_tmp,send_string.c_str());
        if (tcp_ping (serverfd,tmp,send_message_tmp) == 0)
        {
            stats.push_back(tmp);
        }
        //sleep for given useconds
        timespec ts;
        ts.tv_sec = 0;
        ts.tv_nsec = sleep;
        clock_nanosleep(CLOCK_REALTIME, 0, &ts, NULL);

    }
    return stats;
}

int connect_to_server(const char* target, int port)
{
    int sockfd,n;
    struct sockaddr_in servaddr;

    sockfd=socket(AF_INET,SOCK_STREAM,0);

    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr=inet_addr(target);
    servaddr.sin_port=htons(port);

    int i;
    if ((i = connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))<0)
    {
        perror("Connect error");
        ;
    }
    return sockfd;

}

int tcp_ping(int sockfd, long long &time_taken,const char* send_message)
{

    timespec ts;
    timespec ts2;
    clock_gettime(CLOCK_MONOTONIC_RAW,&ts);
    int i,n;

    char recvline[1000];
    if ((i = send(sockfd,send_message,strlen(send_message),0)) < 0)
    {
        perror("sendto error");
    }
    int loop_count =0;
    while (1)
    {
        loop_count++;
        if ( (n = recv(sockfd,recvline,10000,0)) < 0)                  {
            perror("recvfrom error");
            return -1;
        }
        recvline[n] = 0;

        if (strcmp(recvline, send_message)==0)
        {
            break;
        }
    }
    clock_gettime(CLOCK_MONOTONIC_RAW,&ts2);
    time_taken = ((ts2.tv_sec-ts.tv_sec)*SEC2NANO + (ts2.tv_nsec-ts.tv_nsec));

    //cout<<"Loop count is " << loop_count<<endl;
//	cout <<"Received " << recvline << " in " << time_taken << " nano seconds"<<endl;
    return 0;
}


/*int main(int argc, char**argv)
{
	char* target_host;
	int port;
	int sleep_time;
	int num_packets;
	char *send_message;
	char is_non_blocking_str[5];
	bool is_non_blocking = false;

	if (argc != 7)
	{
		printf("usage:  client <IP address> <Port>  <Sleep_time> <num_packets> <send_message> <is_non_blocking> \n");
		exit(1);
	}
	target_host=argv[1];
	port = atoi(argv[2]);
	sleep_time = atoi(argv[3]);
	num_packets = atoi(argv[4]);
	send_message = argv[5];
	strcpy(is_non_blocking_str, argv[6]);

	if (strcmp(is_non_blocking_str,"true")==0)
	{
		is_non_blocking = true;
	}
	vector<int> stats=ping_tcp_server_collect_stats(target_host, port, sleep_time,num_packets, send_message,is_non_blocking);
	std::cout << "The Stats of ping are as below ";
	for (int i =0; i < stats.size(); i++)
		cout<<      stats[i]<<endl;
	std::cout << '\n';
	return 0;
}
*/
