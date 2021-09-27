.PHONY: all clean library program libinstall install

all: library program

libinstall:
	$(MAKE) -C ./lib install

library:
	$(MAKE) -C ./lib all

program:
	$(MAKE) -C ./src all

install: library libinstall program

clean:
	$(MAKE) -C ./lib clean
	$(MAKE) -C ./src clean