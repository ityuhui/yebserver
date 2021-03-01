OBJECTS=main.o initserver.o fileutility.o httprotocol.o interceptor.o
TARGET=yebserver
CFLAGS=-W -g

$(TARGET):$(OBJECTS)
	gcc $(CFLAGS) -o $(TARGET) $(OBJECTS)

main.o:
	gcc $(CFLAGS) -c main.c

initserver.o:
	gcc $(CFLAGS) -c initserver.c

fileutility.o:
	gcc $(CFLAGS) -c fileutility.c

httprotocol.o:
	gcc $(CFLAGS) -c httprotocol.c

interceptor.o:
	gcc $(CFLAGS) -c interceptor.c

.PHONY : clean
clean:
	rm $(OBJECTS) ./$(TARGET)
