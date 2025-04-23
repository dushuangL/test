#ifndef __IP_H__
#define __IP_H__

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

int init_ip(void);
unsigned int pick_dns_ip(void);
const char *pick_domain(void);
int match_dns_ip(unsigned int ip);

#endif
