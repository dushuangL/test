//#include <wcfg_askey.h>
//#include <wcfg_mesh.h>
//#include <wcfg_status.h>
//#include <wlink_mesh.h>
//#include <wlink_wireless.h>
//#include <wwc/timeutil.h>


#include "conf.h"
#include "dns.h"
#include "ping.h"


#if 0
int is_wps_active(void)
{
    bool active = false;
    (void)wlink_wlan_get_wps_status(&active);
    return active ? 1 : 0;
}

int is_mesh_onboarding_active(void)
{
    struct ezmesh_status status = {0};

    (void)wcfg_get_ezmesh_status(&status);
    return (0 == status.mesh_btn_status) ? 0 : 1;
}

int is_connect_internet(void)
{
    struct wcfg_internet_conn_info info = {0};
    (void)wcfg_get_internet_connstatus(&info);
    return info.status ? 1 : 0;
}

int is_init_status(void)
{
    unsigned int init = 0;
    (void)wcfg_get_init_state(&init);
    return (int)init;
}

int is_startup_ready(void)
{
    // mesh套装 域套接字文件存在代表系统启动ok
    static int ready = 0;
    if (0 == ready) {
        ready = wlink_is_mesh_ready();
    }

    return ready;
}

int is_connect_cap(void)
{
    struct ezmesh_mesh_info mesh_info = {0};
    int get_mesh = wcfg_get_ezmesh_meshinfo(&mesh_info);
    if (0 != get_mesh) {
        return 0;
    }

    if (MESH_OB_SUCCESS == mesh_info.onboarding) {
        struct ezmesh_connect_info conn_info = {0};
        int get_conn = wcfg_get_ezmesh_connect_info(&conn_info);
        if (0 == get_conn) {
            return (1 == conn_info.link_status) ? 1 : 0;
        }
    }

    return 0;
}

int is_connect_uplink(void)
{
    struct wcfg_wan_conn_info info = {0};

    (void)wcfg_get_wan_connstatus(&info);
    if (!info.status) {
        unsigned int dns_t0 = 0;
        unsigned int dns_t1 = 0;
        unsigned int ping_t0 = 0;
        unsigned int ping_t1 = 0;

        get_dns_time(&dns_t0, &dns_t1);
        get_ping_time(&ping_t0, &ping_t1);

        //接口释放IP后还能收到ping回包或dns回包，则判定网络上行是连上的
        if ((dns_t1 > info.time) || (ping_t1 > info.time)) {
            return 1;
        }

        //接口释放IP后5秒内，即使没收到ping回包或dns回包，仍认为是连上网络上行的
        unsigned int now = get_uptime_sec();
        if (now <= (info.time + ONLINE_PING_INTERVAL/2)) {
            return 1;
        }
    }

    return info.status ? 1 : 0;
}

int is_led_disabled(void)
{
    bool status = true;
    (void)wcfg_get_led_switch(&status);
    return status ? 0 : 1;
}

int is_bind_app(void)
{
    struct wcfg_askey_onboarding info = {0};
    (void)wcfg_get_askey_onboarding_info(&info);
    return info.onboarded ? 1 : 0;
}

int get_mesh_status(void)
{
    struct ezmesh_mesh_info info = {0};
    (void)wcfg_get_ezmesh_meshinfo(&info);
    return info.onboarding;
}
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <assert.h>
#include "common.h"


#define YEAR_SEC 31536000


//计算从地址addr开始nbytes字节的网络校验和并返回，用于IP/ICMP/IGMP/TCP/UDP协议
unsigned short check_sum(void *addr, unsigned int nbytes)
{
    unsigned int sum = 0;
    unsigned short *ptr = (unsigned short *)addr;

    for (; nbytes > 1; nbytes -= sizeof(*ptr))
    {
        sum += *ptr;
        ++ptr;
    }

    if (nbytes > 0)
    {
        unsigned short tmp = 0;
        *(unsigned char *)(&tmp) = *(unsigned char *)ptr;
        sum += tmp;
    }

    while (sum >> 16)
    {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return (unsigned short)(~sum);
}

unsigned int gen_random_num(void)
{
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd >= 0)
    {
        unsigned int num = 0;
        int cnt = read(fd, &num, sizeof(num));
        close(fd);
        if (sizeof(num) == cnt)
        {
            return num;
        }
    }

    unsigned int seed = (unsigned int)time(NULL) * YEAR_SEC;
    return (unsigned int)rand_r(&seed);
}

//生成满足需求的随机字符串，长度size-1，返回字符串指针，其中pick函数为字符选择函数，满足需求返回非0，不满足返回0
char *gen_random_str(char *buff, unsigned int size, int (*pick)(int c))
{
    assert((NULL != buff) && (size > 0) && (NULL != pick));

    size -= 1;

    unsigned int seed = gen_random_num();
    for (unsigned int i = 0, num = 0; i < size; num >>= 8)
    {
        if (0 == num)
        {
            num = (unsigned int)rand_r(&seed);
        }

        int c = (int)(num & 0x7f);
        if (pick(c))
        {
            buff[i] = (char)c;
            ++i;
        }
    }

    buff[size] = '\0';
    return buff;
}

unsigned int get_uptime_sec(void)
{
    struct sysinfo info = {0};
    (void)sysinfo(&info);
    return (unsigned int)info.uptime;
}

char *gen_time_tag(char *buf, unsigned int size)
{
    assert((NULL != buf) && (size > 0));

    time_t sec = time(NULL);
    struct tm tm = {0};
    (void)localtime_r(&sec, &tm);
    (void)strftime(buf, size, "%Y%m%d-%H%M%S", &tm);

    return buf;
}

//字符分割字符串，字符串全分割，返回分割后的字符串数量
static size_t strsplit(char *str, int c)
{
    assert(NULL != str);

    size_t count = 1;

    for (char *pstr = str; '\0' != *pstr; ++pstr)
    {
        if (c == *pstr)
        {
            *pstr = '\0';
            ++count;
        }
    }

    return count;
}


//将域名转换为DNS报文DNSQR中的QNAME数据
char *domain_to_qname(const char *domain, char *buff, unsigned int size)
{
    assert((NULL != domain) && (NULL != buff) && (0 != size));

    unsigned int len = strlcpy(buff + 1, domain, size - 1);
    if ((len >= (size - 1)) || (0 == len)) {
        return NULL;
    }

    unsigned char *qname = (unsigned char *)buff;
    unsigned int parts = strsplit(buff + 1, '.');
    for (unsigned int i = 0; i < parts; ++i) {
        *qname = strlen((char *)qname + 1);
        qname += *qname + 1;
    }

    return buff;
}

//将DNS报文DNSQR中的QNAME数据转换为域名
char *qname_to_domain(const char *qname, char *buff, unsigned int size)
{
    assert((NULL != qname) && (NULL != buff) && (0 != size));

    unsigned int domain_len = 0;

    domain_len = strlcpy(buff, qname + 1, size);
    if ((domain_len >= size) || (0 == domain_len)) {
        return NULL;
    }

    unsigned int part_len = (unsigned int)*qname;
    char *domain = buff - 1;
    while (1) {
        if ((0 == domain_len) && (0 == part_len)) {
            break;
        }

        if ((0 == domain_len) || (0 == part_len)) {
            return NULL;
        }

        if (domain_len < part_len) {
            return NULL;
        }

        if (domain > buff) {
            *domain = '.';
        }

        domain_len -= part_len;
        domain += part_len + 1;
        part_len = (unsigned int)*domain;
    }

    return buff;
}
