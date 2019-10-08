all: m.o func.o
	gcc -o m -Wall -Werror m.o func.o
m.o:
	gcc -c -Wall -Werror m.c
func.o:
	gcc -c -Wall -Werror func.c
clean:
	rm m ./*.o
