all: clean mem run

clean:
	/bin/rm -rf mem mem.exe *.log

compile: clean mem 

mem: mem.c
	$(CC) -O3 -o mem mem.c

run:  mem
	./mem > mem.log


