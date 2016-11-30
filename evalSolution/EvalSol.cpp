#include <TableAlgorithm.h>

int main(int argc, char **argv)
{
	Problem p;
	Solution s;
	std::ifstream file(argv[1]);
	p = Problem::fromStream(file);

	while (!std::cin.eof()) {
		try {
			std::cin >> s;

			// Evaluate the quality of the solution and
			// it's validity
			std::cout << s << std::endl;
			std::cout << "quality " << p.eval(s)
				  << std::endl;
			
		} catch (std::exception &e) {
			// We ignore lines that are not part of a
			// solution. It is not possible to catch
			// ios_base::failure because of this bug :
			// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=66145

			// TODO Passthrough?
		}
	}

	return 0;
}
