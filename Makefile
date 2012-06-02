OBJECTS=main.o initserver.o fileutility.o httprotocol.o
TARGET=uuwebserver
CFLAGS=-W

$(TARGET):$(OBJECTS)
	gcc $(CFLAGS) -o $(TARGET) $(OBJECTS)
uuwebserver.o:
	gcc $(CFLAGS) -c main.c
initserver.o:
	gcc $(CFLAGS) -c initserver.c
fileutility.o:
	gcc $(CFLAGS) -c fileutility.c
httprotocol.o:
	gcc $(CFLAGS) -c httprotocol.c
clean:
	rm *.o
