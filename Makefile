all: ./main.o ./MLD.o
	gcc -g -o app main.o MLD.o

./main.o: ./main.c
	gcc -g -c main.c -o main.o

./MLD.o: ./MLD.c
	gcc -g -c MLD.c -o MLD.o

clean:
	rm ./*.o