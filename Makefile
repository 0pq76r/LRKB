
all: rkbc.o rkbd.o net.o
	gcc -o rkbc rkbc.o net.o
	gcc -o rkbd rkbd.o net.o
	
%.o:%.c
	gcc -c -o $@ $<

clean:
	rm *.o
