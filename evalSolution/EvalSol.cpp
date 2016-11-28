#include <TableAlgorithm.h>

int main(int argc, char **argv)
{
	Problem p;
	Solution s;
	std::ifstream file(argv[1]);
	p.fromStream(file);

	while (!std::cin.eof()) {
		try {
			std::cin >> s;

			// Evaluate the quality of the solution and
			// it's validity
			std::cout << s; //debug
			
		} catch (...) {
			// We ignore lines that are not part of a
			// solution
		}
	}
	
	return 0;
}
