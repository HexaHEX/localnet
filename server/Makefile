CC=gcc
CFLAGS=-I.
DEPS = broadcaster.h server.h 
LIBS = -pthread
OBJ = broadcaster.o server.o tcpserver.o
TARGET = server

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
