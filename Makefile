CFLAGS = -Wall -Werror -ansi -pedantic

all: rshell

rshell:
	g++ $(CFLAGS) shell.cpp
