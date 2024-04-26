CC=gcc

all: clean dhry run

clean:
	/bin/rm -rf dhry dhry.exe *.log

compile: clean dhry

dhry: dhry_1.c dhry_2.c
	$(CC) -DREG=register -DTIME -O3 -o dhry dhry_1.c dhry_2.c

run:  dhry
	./dhry 1000000000 1
	mv dhry1.log dhry.single.log
	./dhry 1000000000 4
