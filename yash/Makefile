
yash: main.o parse.o actionModule.o signals.o jobs.o
	gcc -o yash main.o parse.o actionModule.o signals.o jobs.o -lreadline 

main.o: main.c parse.h
	gcc -c -g main.c

parse.o: parse.c parse.h
	gcc -c -g parse.c

actionModule.o:actionModule.c actionModule.h
	gcc -c -g actionModule.c

signals.o:signals.c signals.h
	gcc -c -g signals.c
jobs.o: jobs.c jobs.h
	gcc -c -g jobs.c

clean: 
	rm -f yash *.o
