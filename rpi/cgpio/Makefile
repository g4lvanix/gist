CC=gcc

EXEC=cgpio

CFLAGS=-Wall -std=c99

SRC=main.c rpi.c rpi_gpio.c

all: $(SRC)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRC)

clean:
	rm -f *.out *.o $(EXEC)
