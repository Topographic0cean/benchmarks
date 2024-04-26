MAKE=make

all:
	$(MAKE) -C dhrystone 
	$(MAKE) -C whetstone
	$(MAKE) -C disk 
	$(MAKE) -C mem 
	$(MAKE) -C results

clean:
	$(MAKE) -C dhrystone clean
	$(MAKE) -C whetstone clean
	$(MAKE) -C disk clean
	$(MAKE) -C mem clean
	/bin/rm -f *log* win32/*.exe *.zip

compile:
	$(MAKE) -C dhrystone compile  
	$(MAKE) -C whetstone compile
	$(MAKE) -C disk compile 
	$(MAKE) -C mem compile 
	$(MAKE) -C results compiles

run:
	$(MAKE) -C dhrystone run  
	$(MAKE) -C whetstone run
	$(MAKE) -C disk run 
	$(MAKE) -C mem run 
	$(MAKE) -C results run

