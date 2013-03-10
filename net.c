#include "net.h"


int server(int port)
{
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr;
    char flag;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd<0){
		perror("socket");
	}

    memset(&serv_addr, '\0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(port); 

    if(bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))>=0)
    {
		listen(listenfd, 10); 

		while(1)
		{
			connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
			read(connfd, &flag, 1);
			
			printf("Got flag: %x\nCompare with %x\n", flag, RKB_FLAG);

			if(flag==RKB_FLAG)
			{
				return connfd;
			}
			else
			{
				close(connfd);
			}
		 }
	 }
	 else
	 {
		 perror("bind");
		 exit(1);
	 }

	 return -1;
}

int client(char *ip, int port)
{
    int listenfd = 0;
    struct sockaddr_in serv_addr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd<0){
		perror("socket");
	}

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(port); 

	while(1)
	{
		if(connect(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))>=0)
		{
			char buf=RKB_FLAG;
			if(write(listenfd, &buf, 1))
			{
				return listenfd;
			}
		}
		sleep(1);
	}
}
