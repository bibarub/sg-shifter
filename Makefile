CC=gcc
CFLAGS=-Wall
TARGET=sg-shifter

all: $(TARGET)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) sg-shifter.c

clean:
	$(RM) $(TARGET)
