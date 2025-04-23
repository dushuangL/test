#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ip.h"
#include "common.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))

static const char *dns_set[] = {
    "9.9.9.9",        // Quad9 DNS
    "149.112.112.112",// Quad9 DNS
    "8.8.8.8",        // Google DNS
    "8.8.4.4",        // Google DNS
    "4.2.2.1",        // Microsoft DNS
    "4.2.2.2",        // Microsoft DNS
    "1.1.1.1",        // CloudFlare DNS
    "1.0.0.1",        // CloudFlare DNS
    "208.67.222.222", // Cisco OpenDNS
    "208.67.220.220", // Cisco OpenDNS
    "64.6.64.6",      // Verisign Public DNS
    "223.5.5.5",      // Ali DNS
    "223.6.6.6",      // Ali DNS
    "180.76.76.76",   // Baidu DNS
};

static const char *domain_set[] = {
    "walmart.com",
    "amazon.com",
    "apple.com",
    "google.com",
    "microsoft.com",
    "tiktok.com",
    "facebook.com",
    "tesla.com",
    "alibaba.com",
    "samsung.com",
    "toyota.com",
    "vm.com",
    "ericsson.com",
    "byd.com",
};

static unsigned int *ip_set = NULL;
static unsigned int ip_count = 0;

int init_ip(void)
{
    ip_count = ARRAY_SIZE(dns_set);

    if (NULL == ip_set) {
        ip_set = (unsigned int *)calloc(ip_count, sizeof(*ip_set));
        if (NULL == ip_set) {
            printf("calloc ip set fail,err(%d:%s)", errno, strerror(errno));
            return -1;
        }
    }

    for (unsigned int i = 0; i < ip_count; ++i) {
        ip_set[i] = inet_addr(dns_set[i]);
        printf("add dns ip(%03d:%s)", i, inet_ntoa(*(struct in_addr *)&ip_set[i]));
    }

    return 0;
}

unsigned int pick_dns_ip(void)
{
    unsigned int idx = gen_random_num();
    return ip_set[idx % ip_count];
}

const char *pick_domain(void)
{
    unsigned int idx = gen_random_num();
    return domain_set[idx % ARRAY_SIZE(domain_set)];
}

int match_dns_ip(unsigned int ip)
{
    for (unsigned int i = 0; i < ip_count; ++i) {
        if (ip == ip_set[i]) {
            return 0;
        }
    }

    return -1;
}

