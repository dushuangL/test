#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "ping.h"
#include "common.h"

#include "conf.h"
#include "ip.h"
//#include "led_info/led_util.h"

#define OPTION_DATA_LEN (56)

static unsigned int send_uptime = 0;
static unsigned int recv_uptime = 0;


static int ping_sock = 0;

unsigned int online_moment_sec = 0;


int init_ping_time(void)
{
    send_uptime = get_uptime_sec();
    recv_uptime = send_uptime - ONLINE_PING_INTERVAL - 1;
    return 0;
}

void get_ping_time(unsigned int *send_time, unsigned int *recv_time)
{
    *send_time = send_uptime;
    *recv_time = recv_uptime;
}

int init_ping_socket(void)
{
	//int ping_sock = 0;
	ping_sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (ping_sock <= 0) {
		printf("raw socket creation fail,err(%s)", strerror(errno));
		return -1;
	}

	return ping_sock;
}

int ping_sock_fd(void)
{
	return ping_sock;
}

void send_ping_echo(void)
{
		static unsigned short seq = 10;
		static unsigned int pid = 0;
		unsigned char req_pkt[sizeof(struct icmphdr) + OPTION_DATA_LEN] = {0};

		if (0 == pid) {
			pid = getpid();
		}

		struct icmphdr *hdr = (struct icmphdr *)req_pkt;
		char *opt_data = (char *)(hdr + 1);
		(void)gen_random_str(opt_data, OPTION_DATA_LEN, isalnum);
		hdr->type			  = ICMP_ECHO;
		hdr->code			  = 0;
		hdr->un.echo.id 	  = pid;
		hdr->un.echo.sequence = seq++;
		hdr->checksum		  = check_sum(hdr, sizeof(req_pkt));
	
		unsigned int dst_ip = pick_dns_ip();
		struct sockaddr_in addr = {0};
		addr.sin_family 	 = AF_INET;
		addr.sin_port		 = 0;
		addr.sin_addr.s_addr = dst_ip;
		int snd = sendto(ping_sock, req_pkt, sizeof(req_pkt), 0, (struct sockaddr *)&addr, sizeof(addr));
		if (snd <= 0) {
			printf("send ping echo fail,ip(%s)err(%s)", inet_ntoa(*(struct in_addr *)&dst_ip), strerror(errno));
			return;
		}

}

//static int online_moment_sec = 0;

void handle_ping_reply(unsigned int ip)
{
	int match = match_dns_ip(ip);
	 if (0 == match) {
        printf("recv ping reply,ip(%s)", inet_ntoa(*(struct in_addr *)&ip));
        recv_uptime = get_uptime_sec();
        if (0 == online_moment_sec) {
            online_moment_sec = recv_uptime;
            //struct wcfg_internet_conn_info info = {.status = true, .time=recv_uptime,};
            //(void)wcfg_set_internet_connstatus(&info);
        }
    }
}
