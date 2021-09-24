.PHONY: all clean file_reader sender_first sender_second

all: file_reader sender_first sender_second

file_reader:
	$(MAKE) -C ./file_reader all

sender_first:
	$(MAKE) -C ./sender_first all

sender_second:
	$(MAKE) -C ./sender_second all

clean:
	$(MAKE) -C ./file_reader clean
	$(MAKE) -C ./sender_first clean
	$(MAKE) -C ./sender_second clean