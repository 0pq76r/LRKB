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


static int uinput_fd;

int init_uinput(){
	int a=0,b=0;
	
	a = open("/dev/uinput", O_WRONLY | O_NDELAY);
	b = open("/dev/input/uinput", O_WRONLY | O_NDELAY);

	if(a>=0)
	{
		uinput_fd=a;
	}
	else if(b>=0)
	{
		uinput_fd=b;
	}
	else
	{
		return -1;
	}
	
	struct uinput_user_dev dev;
	memset(&dev, 0, sizeof(struct uinput_user_dev));
	strcpy(dev.name, "RKBD");
	
	ioctl(uinput_fd, UI_SET_EVBIT, EV_KEY);
	
	for(a=0;a<256;a++)
	{
		ioctl(uinput_fd, UI_SET_KEYBIT, a);
	}
	
	write(uinput_fd, &dev, sizeof(struct uinput_user_dev));
	
	return ioctl(uinput_fd, UI_DEV_CREATE)?-1:0;

}

void destroy_uinput(){
	ioctl(uinput_fd, UI_DEV_DESTROY);
	close(uinput_fd);
}


void press_key(int keycode){
	struct input_event ev;
	memset(&ev, 0, sizeof(struct input_event));
	
	gettimeofday(&ev.time, NULL);
	ev.type=EV_KEY;
	ev.code=keycode;
	ev.value=1;
	
	write(uinput_fd, &ev, sizeof(struct input_event));

	ev.type = EV_SYN;
	ev.code = SYN_REPORT;
	ev.value = 0;
	write(uinput_fd, &ev, sizeof(struct input_event));
}

void release_key(int keycode){
	struct input_event ev;
	memset(&ev, 0, sizeof(struct input_event));
	
	gettimeofday(&ev.time, NULL);
	ev.type=EV_KEY;
	ev.code=keycode;
	ev.value=0;
	
	write(uinput_fd, &ev, sizeof(struct input_event));

	ev.type = EV_SYN;
	ev.code = SYN_REPORT;
	ev.value = 0;
	write(uinput_fd, &ev, sizeof(struct input_event));
}

int main(int argc, char **argv)
{
	char keys[1024]={0};
	
	if(init_uinput()<0)
	{
		perror("init_uinput failed");
		return -1;
	}
	
	int src=server(12345);
	int data;
	
	while(1)
	{
		read(src, &data, 4);
		
		if(data<1024)
		{
			printf("Got Keycode: %i\n", data);
			keys[data]^=1;
			if(keys[data])
			{
				press_key(data);
			}
			else
			{
				release_key(data);
			}			
		}
	}
	
	
	destroy_uinput();

	return 0;
}
