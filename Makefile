all :
	make -C common
	make -C runAlgo
	make -C evalSolution

clean :
	make clean -C common
	make clean -C runAlgo
	make clean -C evalSolution
