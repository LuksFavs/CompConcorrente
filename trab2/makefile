programa: main.o arrumar.o
	gcc main.o arrumar.o -o programa -Wall -lm

arrumar.o: arrumar.c arrumar.h
	gcc -c arrumar.c -o arrumar.o -Wall -lm

main.o: main.c arrumar.h 
	gcc -c main.c -o main.o -Wall -lm


