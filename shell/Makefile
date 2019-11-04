all: m.o cmdline.o list.o
	gcc *.o -o m
m.o:
	gcc -c m.c
cmdline.o:
	gcc -c tools/cmdline/cmdline.c
list.o:
	gcc -c tools/list/list.c
clean:
	rm -rf *.o m
