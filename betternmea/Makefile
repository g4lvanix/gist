CC=gcc

TARGET=main

CFLAGS = -Wall -std=c99 -Wno-unused-function
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -I./
LDFLAGS = -Wl,--gc-sections

OBJS = main.o
OBJS += nmea.o

all: $(TARGET)

clean: 
	rm -f $(TARGET) $(OBJS)

$(TARGET): $(OBJS)
