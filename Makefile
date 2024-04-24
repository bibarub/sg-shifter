CC=gcc
CFLAGS=-Wall
TARGET=sg-shifter

all: $(TARGET)

$(TARGET): sg-shifter.c
	$(CC) $(CFLAGS) -o $(TARGET) sg-shifter.c

clean:
	$(RM) $(TARGET)
