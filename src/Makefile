CC:=gcc
CFLAGS:=-Wall

LIBUSED:=-llogger
PREFIX?=/usr

.PHONY: all clean install uninstall

all: logging_enjoyer logging_fan

logging_enjoyer: logging_enjoyer.o
	$(CC) $(CFLAGS) -o $@ $^ -L$(PWD) -llogger

logging_fan: logging_fan.o
	$(CC) $(CFLAGS) -o $@ $^ -L$(PWD) -llogger
	

clean:
	$(RM) $(OBJS)