#*******************************************************************************
.PHONY: all build clean install uninstall
all: build
build: binary-read binary-write
#===============================================================================
CFLAGS := -O3
LDFALGS := -s
#===============================================================================
binary-read: cmd_arg.c binary-read.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
binary-write: cmd_arg.c binary-write.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
#-------------------------------------------------------------------------------
clean:
	rm -f binary-{read,write}
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
install:
	install -C -o root -g root -m 755 binary-read /usr/bin/binary-read
	install -C -o root -g root -m 755 binary-write /usr/bin/binary-write
uninstall:
	rm -f /usr/bin/binary-{read,write}
#*******************************************************************************