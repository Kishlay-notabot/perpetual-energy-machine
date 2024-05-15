
CC = gcc

lure: samlib.o src/main.c
	$(CC) -o lure $(CFLAGS) src/main.c samlib.o

pull:
	git submodule update --init

samlib.o: reciter.o sam.o render.o processframes.o createtransitions.o debug.o
	ld -r -o samlib.o processframes.o createtransitions.o reciter.o sam.o render.o debug.o

%.o: src/SAM/src/%.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm *.o
