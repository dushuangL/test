#ifndef __PING_H__
#define __PING_H__

int init_ping(void);
int init_ping_time(void);
int init_ping_socket(void);
int ping_sock_fd(void);
void get_ping_time(unsigned int *send_time, unsigned int *recv_time);
void send_ping_echo(void);
void handle_ping_reply(unsigned int ip);

#endif // !__PING_H__
