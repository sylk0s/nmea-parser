CFLAGS = -g
LDFLAGS =
CC = gcc
EXES = nmea test

# rules are "target: <dependencies>" followed by zero or more
# lines of actions to create the target
#
all: $(EXES)

nmea: nmea.o test.o
	$(CC) -g $^ -o $@ $(LDFLAGS)

clean:
	rm -f *.o $(EXES)
