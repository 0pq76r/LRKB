#ifndef __NET_H__
#define __NET_H__

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 

#define RKB_FLAG 0x12

extern int server(int port);
extern int client(char *ip, int port);

#endif
