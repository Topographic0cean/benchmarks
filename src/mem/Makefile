all: clean mem run

clean:
	/bin/rm -rf mem mem.exe *.log mem.dSYM

compile: clean mem 

mem: mem.c
	$(CC) -g -o mem mem.c

run:  mem
	./mem > mem.log


