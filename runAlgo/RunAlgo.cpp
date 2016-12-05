#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

#include "TableAlgorithm.h"
#include "TrivialAlgo.h"
#include "AlgoCotton1.h"
#include "AlgoCotton2.h"
#include "GraphAlgorithm.h"


// Simple argument parsing inspired from
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
	const int optw = 35;
	std::cout << "Usage : " << progname
		  << " [-h] [-p] [-t] [-e DATAFILE] [-a ALGO]"
		  << std::endl;
	std::cout << "OPTIONS:" << std::endl;
	std::cout << std::setw(optw) << "[ -h/--help]"
		  << "  Prints this help." << std::endl;
	std::cout << std::setw(optw) << "[ -p/--print]"
		  << "  Prints the solutions." << std::endl;
	std::cout << std::setw(optw) << "[ -t/--time]"
		  << "  Prints the time needed to find every solution."
		  << std::endl;
	std::cout << std::setw(optw) << "[ -e DATAFILE | --data "
		"DATAFILE]"
		  << "  The input problem file, defaults to stdin."
		  << std::endl;
	std::cout << std::setw(optw) << "[ -a ALGO | --algo ALGO]"
		  << "  The algorithm to use." << std::endl;
}

void print_algos()
{
	std::cout << "The possible algorithms are : " << std::endl;
	std::cout << "\t {";
	auto list = TableAlgorithm::list();
	for (auto it = list.begin(); it != list.end(); ++it)
		std::cout << *it << ((it==list.end()-1) ? "}" : ", ");
	std::cout << std::endl;
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
	char* filepath = getOptArg(argc, argv, "-e") ?:
		getOptArg(argc, argv, "--data");
	char* algoname = getOptArg(argc, argv, "-a") ?:
		getOptArg(argc, argv, "--algo");

	// initialize algorithms
	// FIXME should this be somewhere else
	TableAlgorithm::addAlgorithm(new TrivialAlgo::factory());
	TableAlgorithm::addAlgorithm(new AlgoCotton1::factory());
	TableAlgorithm::addAlgorithm(new AlgoCotton2::factory());
	TableAlgorithm::addAlgorithm(new GraphAlgorithm::factory());

	// Choose algorithm
	std::unique_ptr<TableAlgorithm> algorithm;
	if (algoname)
		algorithm = TableAlgorithm::make(algoname);
	else
		algorithm = std::unique_ptr<TableAlgorithm>(new TrivialAlgo());

	if (help || !algorithm){
		usage(argv[0]);
		print_algos();
		return 1;
	}

	// Read problem
	Problem p;
	if (filepath) {
		std::ifstream in = std::ifstream(filepath);
		p = Problem::fromStream(in);
	} else {
		p = Problem::fromStream(std::cin);
	}

	// Run algo
	TableAlgorithm::run_cb_t cb =
		[print, time](
			const Solution &s,
			const std::chrono::duration<float> &elapsed)
		{
			if (print)
				std::cout << s << std::endl;
			if (time)
				std::cout << "time : "
					  << elapsed.count()
					  << 's'
					  << std::endl;
		};
	algorithm->run(p, cb);

	return 0;
}
