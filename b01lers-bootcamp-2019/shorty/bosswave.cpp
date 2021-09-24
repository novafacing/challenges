#include <cstddef>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <streambuf>
#include <string>
#include <vector>

int main(int argc, char ** argv) {
	if (argc != 2) {
		::exit(0);
	}
	std::string fname(argv[1]);
	std::ifstream flagfile(fname);
	std::vector<char> flag((std::istreambuf_iterator<char>(flagfile)), std::istreambuf_iterator<char>());
	flag.pop_back();
	std::time_t seed_time = std::time(0);
	auto local_seed_time  = *std::localtime(&seed_time);
	std::cout << std::put_time(&local_seed_time, "%d-%m-%Y %H-%M-%S")  << std::endl;
	std::mt19937 mt_rand(seed_time);
	for (int i = 0; i < 624; i++) {
		std::cout << "SKIPPING: " << mt_rand() << std::endl;
	}
	for (int i = 0; i < 512; i++) {
		long int swap_a = mt_rand();
		long int swap_b = mt_rand();
		char temp = flag[swap_a % flag.size()];
		flag[swap_a % flag.size()] = flag[swap_b % flag.size()];
		flag[swap_b % flag.size()] = temp;
	}
	std::string flag_out(flag.begin(), flag.end());
	std::cout << flag_out;
	fflush(stdout);
}
