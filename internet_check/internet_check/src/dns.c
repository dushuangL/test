#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>

#include "conf.h"
#include "ip.h"
#include "common.h"
#include "dns.h"
//#include "led_info/led_util.h"

struct dnshdr {
    unsigned short transid; //事务ID
    unsigned short flags;   //标志位
    unsigned short qdcount; //questions问题数量
    unsigned short ancount; //answers回答数量
    unsigned short nscount; //authorities授权数量
    unsigned short arcount; //additional附加数量
};

static unsigned int send_uptime = 0;
static unsigned int recv_uptime = 0;
static int dns_sock;


int init_dns_time(void)
{
    send_uptime = get_uptime_sec();
    recv_uptime = send_uptime - ONLINE_PING_INTERVAL - 1;
    return 0;
}

void get_dns_time(unsigned int *send_time, unsigned int *recv_time)
{
    *send_time = send_uptime;
    *recv_time = recv_uptime;
}

int init_dns_socket(void)
{
    dns_sock = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC | SOCK_NONBLOCK, 0);
    if (dns_sock < 0) {
        printf("create socket fail,err(%s)", strerror(errno));
        return -1;
    }

	return dns_sock;
}

void send_dns_query(const char *domain)
{
    //static int fd = INVALID_SOCKET;
    char buff[512] = {0};
    struct dnshdr *hdr = (struct dnshdr *)buff;
    char *query = NULL;
    unsigned int packet_len = 0;

    send_uptime = get_uptime_sec();


    packet_len += sizeof(struct dnshdr);
    if (packet_len > sizeof(buff)) {
        printf("compose dns header fail, buff size is too small");
        return;
    }

    unsigned short id = (unsigned short)gen_random_num();
    hdr->transid = htons(id);
    hdr->flags   = 0;        //dns query报文且非递归请求
    hdr->qdcount = htons(1); //请求1个域名



    query = domain_to_qname(domain, buff + sizeof(struct dnshdr), sizeof(buff) - packet_len);
    if (NULL == query) {
        printf("compose dns query fail");
        return;
    }

    packet_len += strlen(query) + 1;
    if ((packet_len + (sizeof(unsigned short) * 2)) > sizeof(buff)) {
        printf("compose dns query fail, buff size is too small");
        return;
    }

    unsigned short *qtype  = (unsigned short *)(buff + packet_len);
    unsigned short *qclass = qtype + 1;
    *qtype  = htons(1); //A类映射，即IPv4地址
    *qclass = htons(1); //IN类型，即互联网Internet上域名

    packet_len += sizeof(unsigned short) * 2;
    unsigned int dns_ip = pick_dns_ip();
    struct sockaddr_in addr = {0};
    addr.sin_family      = AF_INET;
    addr.sin_port        = htons(53);
    addr.sin_addr.s_addr = dns_ip;
    int snd = sendto(dns_sock, buff, packet_len, 0, (struct sockaddr *)&addr, sizeof(addr));
    if (snd < 0) {
        printf("send dns query(%s) fail,err(%s)", inet_ntoa(*(struct in_addr *)&dns_ip), strerror(errno));
        return;
    }
}

static unsigned int online_moment_sec = 0;

void handle_dns_response(unsigned int ip)
{
    int match = match_dns_ip(ip);
    if (0 == match) {
        printf("recv dns response,ip(%s)", inet_ntoa(*(struct in_addr *)&ip));
        recv_uptime = get_uptime_sec();
        if (0 == online_moment_sec) {
            online_moment_sec = recv_uptime;
           // struct wcfg_internet_conn_info info = {.status = true, .time=recv_uptime,};
            //(void)wcfg_set_internet_connstatus(&info);
        }

    }
}
