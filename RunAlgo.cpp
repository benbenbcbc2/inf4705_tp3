#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>

#include "TableAlgorithm.h"

// Simple argument parsing from
// http://stackoverflow.com/a/868894/2041995
char* getOptArg(int argc, char** argv, const std::string &opt)
{
	char **begin = argv, **end = argv + argc;
	char **res = std::find(begin, end, opt);
	if (res != end && ++res != end)
		return *res;
	return 0;
}

bool hasOpt(int argc, char** argv, const std::string &opt)
{
	return std::find(argv, argv+argc, opt) != (argv+argc);
}

void usage(char *progname)
{
	std::cout << "Usage : " << progname
		  << " [-h] [-p] [-t] [-f FILE] [-a ALGO]"
		  << std::endl;
	std::cout << "OPTIONS:" << std::endl;
	std::cout << std::setw(30) << "[ -h/--help]"
		  << "  Prints this help." << std::endl;
	std::cout << std::setw(30) << "[ -p/--print]"
		  << "  Prints the solutions." << std::endl;
	std::cout << std::setw(30) << "[ -t/--time]"
		  << "  Prints the time needed to find every solution."
		  << std::endl;
	std::cout << std::setw(30) << "[ -f FILE | --file FILE]"
		  << "  The input problem file, defaults to stdin."
		  << std::endl;
	std::cout << std::setw(30) << "[ -a ALGO | --algo ALGO]"
		  << "  The algorithm to use." << std::endl;
}

int main(int argc, char** argv)
{
	// Parse arguments
	bool help = hasOpt(argc, argv, "-h") ||
		hasOpt(argc, argv, "--help");
	bool print = hasOpt(argc, argv, "-p") ||
		hasOpt(argc, argv, "--print");
	bool time = hasOpt(argc, argv, "-t") ||
		hasOpt(argc, argv, "--time");
	char* filepath = getOptArg(argc, argv, "-f") ?:
		getOptArg(argc, argv, "--file");
	char* algoname = getOptArg(argc, argv, "-a") ?:
		getOptArg(argc, argv, "--algo");

	// Choose algorithm
	std::unique_ptr<TableAlgorithm> algorithm;
	if (algoname)
		algorithm = TableAlgorithm::make(algoname);
	else
		algorithm = std::unique_ptr<TableAlgorithm>(new TrivialAlgo());
	
	if (help){
		usage(argv[0]);
		return 1;
	}

	// Read problem
	std::unique_ptr<Problem> p;
	if (filepath) {
		std::ifstream in = std::ifstream(filepath);
		p = Problem::fromStream(in);
	} else {
		p = Problem::fromStream(std::cin);
	}

	// Run algo
	run_cb_t cb =
		[print, time](
			const Solution &s,
			const std::chrono::duration<float> &elapsed)
		{
			if (print)
				std::cout << s;
			if (time)
				std::cout << "time (s)"
					  << elapsed.count()
					  << std::endl;
		};
	algorithm->run(*p, cb);
	
	return 0;
}
