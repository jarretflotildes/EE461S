
yash: main.o parse.o actionModule.o
	gcc -o yash main.o parse.o actionModule.o -lreadline 

main.o: main.c parse.h
	gcc -c -g main.c

parse.o: parse.c parse.h
	gcc -c -g parse.c

actionModule.o:actionModule.c actionModule.h
	gcc -c -g actionModule.c
