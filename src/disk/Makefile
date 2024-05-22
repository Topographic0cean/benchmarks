all: clean disk run

clean:
	/bin/rm -rf disk disk.exe *.log testdir

compile: clean disk

disk: disk.c
	gcc -O3 -D_GNU_SOURCE -o disk disk.c

run:  disk
	./disk $(TESTDIR) 1G > disk.log 
	/bin/rm $(TESTDIR)/testfile
