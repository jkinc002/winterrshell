CFLAGS = -Wall -Werror -ansi -pedantic

all: rshell

rshell:
	g++ $(CFLAGS) src/shell.cpp
