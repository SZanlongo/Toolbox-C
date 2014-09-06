all: test
	
test: main.o
	gcc -Wall main.o -lpthread -o test
	
main.o : main.c
	gcc -g -c -Wall main.c -lpthread #-o main.o

clean:
	$(RM) *o test
