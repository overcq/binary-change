.PHONY: all build install
all: build
build:
	$(CC) -s -O2 cmd_arg.c main.c
install:
	gksu ' \
		install -C -o root -g root -m 755 a.out /usr/bin/binary-change \
	'
