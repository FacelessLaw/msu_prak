all: m.o mattrix.o
	gcc -o m -Wall -Werror m.o mattrix.o
m.o:
	gcc -c m.c
mattrix.o:
	gcc -c lib/mattrix.c
clean:
	rm m mattrix.o m.o
