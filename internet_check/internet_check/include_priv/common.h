#ifndef __COMMON_H__
#define __COMMON_H__

unsigned short check_sum(void *addr, unsigned int nbytes);

unsigned int gen_random_num(void);
char *gen_random_str(char *buff, unsigned int size, int (*pick)(int c));
unsigned int get_uptime_sec(void);
char *gen_time_tag(char *buf, unsigned int size);
char *domain_to_qname(const char *domain, char *buff, unsigned int size);
char *qname_to_domain(const char *qname, char *buff, unsigned int size);
int is_led_disabled(void);
int is_bind_app(void);
int is_startup_ready(void);
int get_mesh_status(void);

#endif
