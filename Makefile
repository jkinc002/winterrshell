CFLAGS = -Wall -Werror -ansi -pedantic

all: bin rshell

bin:
	[ ! -d $bin ] && mkdir bin

rshell:
	g++ $(CFLAGS) src/shell.cpp -o ./bin/rshell

clean:
	rm -f *.0 rshell
	rm -rf *.0 bin
	echo Clean done
