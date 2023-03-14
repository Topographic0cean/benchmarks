run: whetstone
	./whetstone 1000000 > whet.log


whetstone: whetstone.c
	gcc -O3 -o whetstone whetstone.c -lm



clean:
	/bin/rm -rf whetstone *.log
