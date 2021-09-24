CC:=gcc
CFLAGS:=-Wall -Ipwd
SRC:=$(wildcard *.c)
OBJ:=$(SRC:.c=.o)
BIN:=file_reader
LNAME:=llogger

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ -$(LNAME)

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) $(BIN) $(OBJ) 