CC:=gcc
CFLAGS:=-Wall
EXTRAFLAGS:=-fPIC
LDFLAGS:=-shared
SRC:=$(wildcard *.c)
OBJS:=$(SRC:.c=.o)
INCLUDES:=logger.h
LIB:=liblogger.so

PREFIX?=/usr

.PHONY: all clean install uninstall

all: $(LIB)

$(LIB): $(OBJS)
	$(CC) $(CFLAGS) $(EXTRAFLAGS) -o $@ $(LDFLAGS) $(OBJS)

%.o:%.c
	$(CC) $(CFLAGS) $(EXTRAFLAGS) -o $@ $^ $(LDFLAGS) -lsqlite3

clean:
	$(RM) $(LIB)
	$(RM) $(OBJS)

install: $(LIB)
	install -d $(PREFIX)/include
	install -d $(PREFIX)/lib
	install -m 644 $(INCLUDES) $(PREFIX)/include
	install -m 644 $(LIB) $(PREFIX)/lib

uninstall:
	$(RM) $(PREFIX)/include/$(INCLUDES)
	$(RM) $(PREFIX)/lib/$(LIB)