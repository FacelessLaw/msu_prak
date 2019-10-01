all: m.o cat.o my_string.o parse_word.o shl_sort.o wc.o
	gcc *.o -o m
m.o:
	gcc -c m.c
cat.o:
	gcc -c tools/cat/cat.c
my_string.o:
	gcc -c tools/my_string/my_string.c
parse_word.o:
	gcc -c tools/parse_word/parse_word.c
shl_sort.o:
	gcc -c tools/shl_sort/shl_sort.c
wc.o:
	gcc -c tools/wc/wc.c
clean:
	rm -rf *.o m
