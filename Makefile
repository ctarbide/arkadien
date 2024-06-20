
CC = gcc
CFLAGS = -Wall -O2 -c
LDFLAGS = -lm

PROG = arkadien

$(PROG): $(PROG).o arc.o pcg.o
	$(CC) -o $(PROG) $(PROG).o arc.o pcg.o $(LDFLAGS)

$(PROG).o: $(PROG).c arc.h
	$(CC) $(CFLAGS) $(PROG).c

pcg.o: pcg.c pcg.h
	$(CC) $(CFLAGS) pcg.c

arc.o: arc.c arc.h library.h
	$(CC) $(CFLAGS) arc.c

library.h: library.arc
	echo '#define stdlib_LAST_ITEM ,0' >library.h
	NAME=stdlib NAME_TYPE_QUALIFIER='static' ./c-hexify.sh \
	    <library.arc >>library.h

run: $(PROG)
	./$(PROG)

clean:
	rm -f $(PROG) library.h *.o
