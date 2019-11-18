all: m.o cmdline.o list.o runproc.o tree.o
	gcc *.o -o m
m.o:
	gcc -c m.c
cmdline.o:
	gcc -c tools/cmdline/cmdline.c
list.o:
	gcc -c tools/list/list.c
runproc.o:
	gcc -c tools/runproc/runproc.c
tree.o:
	gcc -c tools/tree/tree.c
clean:
	rm -rf *.o m
