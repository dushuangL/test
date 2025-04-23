#ifndef __DNS_H__
#define __DNS_H__

int init_dns(void);
int init_dns_time(void);
void get_dns_time(unsigned int *send_time, unsigned int *recv_time);
int init_dns_socket(void);
void send_dns_query(const char *domain);
void handle_dns_response(unsigned int ip);

#endif // !__DNS_H__
