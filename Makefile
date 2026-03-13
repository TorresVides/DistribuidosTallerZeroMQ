CC = gcc
CFLAGS = $(shell pkgconf --cflags libzmq)
LIBS = $(shell pkgconf --libs libzmq)

TARGETS = coordinador.exe worker.exe


all: $(TARGETS)

coordinador.exe: coordinador.c
	$(CC) coordinador.c -o coordinador.exe $(CFLAGS) $(LIBS)

worker.exe: worker.c
	$(CC) worker.c -o worker.exe $(CFLAGS) $(LIBS)

clean:
	rm -f *.exe

rebuild: clean all
