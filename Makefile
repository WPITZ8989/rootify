CFLAGS = -Wall -O2
rootify: rootify.c
	$(CC) $(CFLAGS) -w -o rootify rootify.c

install: rootify
	cp -f rootify /usr/local/bin
	chown root: /usr/local/bin/rootify
	chmod 6755 /usr/local/bin/rootify

clean:
	rm rootify

