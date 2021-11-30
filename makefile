all: main.o commands.h commands.o 
	gcc -o main main.o commands.o

commands.o: commands.h commands.c
	gcc -c commands.c

main.o: main.c commands.h commands.o
	gcc -c main.c

run:
	./main

clean:
	rm *.o