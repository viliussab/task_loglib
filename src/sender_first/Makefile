CC:=gcc
CFLAGS:=-Wall -Ipwd
SRC:=$(wildcard *.c)
OBJ:=$(SRC:.c=.o)
BIN:=sender_first
LNAME:=llogger

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -$(LNAME)

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) $(BIN) $(OBJ) 