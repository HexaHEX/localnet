CC=g++
CFLAGS=-I.
DEPS = client.h udplistener.h 
LIBS = -pthread
OBJ = udplistener.o client.o tcpclient.o
TARGET = client

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
