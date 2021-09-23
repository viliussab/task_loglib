.PHONY: all clean library program

all: library program

library:
	$(MAKE) -C ./lib all install

program:
	$(MAKE) -C ./src all

clean:
	$(MAKE) -C ./lib clean
	$(MAKE) -C ./src clean