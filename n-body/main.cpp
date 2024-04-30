#include <stdexcept>
#include <stdlib.h>

#include "simulation.h"

using namespace::std;

constexpr auto DEFAULT_BODIES_COUNT = 1000;

size_t parse_args(int argc, char* argv[]);

int main(int argc, char* argv[])
{
	auto start = clock();

	auto sim = simulation(parse_args(argc, argv));
	sim.run(0.1, 1.0);

	cout << "Total time: " << clock() - start << " ms" << endl;

	return 0;
}

size_t parse_args(int argc, char* argv[])
{
	switch (argc)
	{
	case 1:
		return DEFAULT_BODIES_COUNT;
	case 2:
		return atoi(argv[1]);
	default:
		throw std::invalid_argument("invalid command line args");
	}
}