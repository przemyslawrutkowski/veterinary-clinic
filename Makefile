CC = gcc
override CFLAGS += -g -Wno-everything

SRCS = main.c cJSON.c

main: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o main

clean:
	rm -f main