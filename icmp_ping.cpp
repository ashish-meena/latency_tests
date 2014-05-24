#include "icmp_ping.h"

#define	DEFDATALEN	(64-ICMP_MINLEN)	/* default data length */
#define	MAXIPLEN	60
#define	MAXICMPLEN	76
#define	MAXPACKET	(65536 - 60 - ICMP_MINLEN)/* max packet size */
#define SEC2NANO 1000000000.0f


vector<int> ping_server_collect_stats(string target, int num_packets)
{
    vector<int> stats;
    int socket_fd;
    struct sockaddr_in sockaddr = create_raw_socket(target, socket_fd);
    for (int i = 0; i < num_packets; i++)
    {
        long long tmp = 10000000000000000;

        if (ping (sockaddr,socket_fd,i+1,tmp) == 0)
        {
//cout << "pushed" << tmp<<endl;
            stats.push_back(tmp);
        }
    }
    close(socket_fd);
    return stats;
}

struct sockaddr_in create_raw_socket(string target, int &socket_fd)
{
    int s;
    struct hostent *hp;
    char hnamebuf[MAXHOSTNAMELEN];
    string hostname;
    struct sockaddr_in to;
    to.sin_family = AF_INET;
    // try to convert as dotted decimal address, else if that fails assume it's a hostname
    to.sin_addr.s_addr = inet_addr(target.c_str());
    if (to.sin_addr.s_addr != (u_int)-1)
        hostname = target;
    else
    {
        hp = gethostbyname(target.c_str());
        if (!hp)
        {
            perror("Unknown Host");
            exit(-1);
        }
        to.sin_family = hp->h_addrtype;
        bcopy(hp->h_addr, (caddr_t)&to.sin_addr, hp->h_length);
        strncpy(hnamebuf, hp->h_name, sizeof(hnamebuf) - 1);
        hostname = hnamebuf;
    }


    if ( (s = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
    {
        perror("socket");	/* probably not running as superuser */
        exit (-1);
    }
    socket_fd=s;
    return to;

}
//returns 0 if ping is successful and stores total taken time in TimeTaken variable


int ping(sockaddr_in to,int socket_fd, int seq_num, long long &time_taken)
{   int i,cc, packlen, datalen = DEFDATALEN;
    struct icmp *icp;
    struct ip *ip;
    struct sockaddr_in from;
    int ret, fromlen, hlen;
    fd_set rfds;
    struct timeval tv;
    int retval;
    struct timeval start, end;
    bool cont = true;

    packlen = datalen + MAXIPLEN + MAXICMPLEN;
    u_char *packet, outpack[MAXPACKET];
    if ( (packet = (u_char *)malloc((u_int)packlen)) == NULL)
    {
        cerr << "malloc error\n";
        return -1;
    }
    icp = (struct icmp *)outpack;
    icp->icmp_type = ICMP_ECHO;
    icp->icmp_code = 0;
    icp->icmp_cksum = 0;
    icp->icmp_seq = seq_num;	/* seq and id must be reflected */
    icp->icmp_id = getpid();


    cc = datalen + ICMP_MINLEN;
    icp->icmp_cksum = in_cksum((unsigned short *)icp,cc);

    timespec ts;
    timespec ts2;
    clock_gettime(CLOCK_MONOTONIC_RAW,&ts);

    i = sendto(socket_fd, (char *)outpack, cc, 0, (struct sockaddr*)&to, (socklen_t)sizeof(struct sockaddr_in));
    if (i < 0 || i != cc)
    {
        if (i < 0)
            perror("sendto error");
    }

    // Watch stdin (fd 0) to see when it has input.
    FD_ZERO(&rfds);
    FD_SET(socket_fd, &rfds);
    // Wait up to one seconds.
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    while(cont)
    {
        retval = select(socket_fd+1, &rfds, NULL, NULL, &tv);
        if (retval == -1)
        {
            perror("select()");
            return -1;
        }
        else if (retval)
        {
            fromlen = sizeof(sockaddr_in);
            if ( (ret = recvfrom(socket_fd, (char *)packet, packlen, 0,(struct sockaddr *)&from, (socklen_t*)&fromlen)) < 0)
            {
                perror("recvfrom error");
                return -1;
            }

            // Check the IP header
            ip = (struct ip *)((char*)packet);
            hlen = sizeof( struct ip );
            if (ret < (hlen + ICMP_MINLEN))
            {
                perror ("packet too short ");
                return -1;
            }

            // Now the ICMP part
            icp = (struct icmp *)(packet + hlen);
            if (icp->icmp_type == ICMP_ECHOREPLY)
            {
                if (icp->icmp_seq != seq_num)
                {
                    cout << "received sequence # " << icp->icmp_seq << endl;
                    continue;
                }
                if (icp->icmp_id != getpid())
                {
                    cout << "received id " << icp->icmp_id << endl;
                    continue;
                }
                cont = false;
            }
            else
            {
                //			cout << "Recv: not an echo reply" << endl;
                continue;
            }

            clock_gettime(CLOCK_MONOTONIC_RAW,&ts2);

            time_taken = ((ts2.tv_sec-ts.tv_sec)*SEC2NANO +
                          (ts2.tv_nsec-ts.tv_nsec));
//cout<<"time taken for seq_num" << seq_num << " was " << time_taken <<endl;
        }
        else
        {
//			cout << "No data within one seconds.\n";
            return 0;
        }
    }
    return 0;
}

uint16_t in_cksum(uint16_t *addr, unsigned len)
{
    uint16_t answer = 0;
    /*
     * Our algorithm is simple, using a 32 bit accumulator (sum), we add
     * sequential 16 bit words to it, and at the end, fold back all the
     * carry bits from the top 16 bits into the lower 16 bits.
     */
    uint32_t sum = 0;
    while (len > 1)  {
        sum += *addr++;
        len -= 2;
    }

    // mop up an odd byte, if necessary
    if (len == 1) {
        *(unsigned char *)&answer = *(unsigned char *)addr ;
        sum += answer;
    }

    // add back carry outs from top 16 bits to low 16 bits
    sum = (sum >> 16) + (sum & 0xffff); // add high 16 to low 16
    sum += (sum >> 16); // add carry
    answer = ~sum; // truncate to 16 bits
    return answer;
}


/*
int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cout << "Usage: ping hostname" << endl;
		exit(1);
	}
long long nanoSeconds;
vector<int> stats = ping_server_collect_stats(argv[1], atoi(argv[2]));


std::cout << "The Stats of ping are as below ";
for (int i =0; i < stats.size(); i++)
	cout<<      stats[i]<<endl;
  std::cout << '\n';
	return 0;
}*/
