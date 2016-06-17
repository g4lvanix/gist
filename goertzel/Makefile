TARGET=main
OBJS=goertzel.o

CFLAGS=-Wall -std=gnu99 -I.
LDFLAGS=-lm

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
