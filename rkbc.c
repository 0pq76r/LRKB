#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include "net.h"

static int input_fd;

int init_input(char *input_dev){	
	input_fd = open(input_dev, 0);

	if(input_fd>=0)
	{
		return 0;
	}
	return -1;
}

void destroy_uinput(){
	close(input_fd);
}


int read_key(){
	struct input_event ev;
	memset(&ev, 0, sizeof(struct input_event));	
	while((ev.type!=EV_KEY)&&(ev.type!=EV_REL)&&(ev.type!=EV_ABS))
	{
		read(input_fd, &ev, sizeof(struct input_event));
	}
	return ev.code;
}

int main(int argc, char **argv)
{
	if(init_input(argv[1])<0)
	{
		perror("init_uinput");
		return -1;
	}
	
	int dst=client("192.168.1.228", 12345);
	
	while(1)
	{
		int key=read_key();
		printf("Send Keycode: %i\n", key);

		if(!write(dst, &key, 4))
		{
			perror("write");
		}
	}
	
	
	destroy_uinput();

	return 0;
}
