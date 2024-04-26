
benchmark:
	make -C src	

database:
	cd data; ./package
    


clean:
	make -C src clean
