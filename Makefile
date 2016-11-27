all :
	make -C common
	make -C runAlgo

clean :
	make clean -C common
	make clean -C runAlgo
