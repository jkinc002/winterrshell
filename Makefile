CFLAGS = -Wall -Werror -ansi -pedantic

all: bin rshell ls

bin:
	[ ! -d $bin ] && mkdir bin

rshell:
	g++ $(CFLAGS) src/shell.cpp -o ./bin/rshell

ls:
	g++ $(CFLAGS) src/ls.cpp -o ./bin/ls

clean:
	rm -f *.0 rshell
	rm -f *.0 ls
	rm -rf *.0 bin
	echo Clean done
