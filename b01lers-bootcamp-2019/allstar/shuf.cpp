#include <vector>
#include <random>
#include <iostream>

int main() {
	std::vector<uint8_t> mod = { 70, 47, 37, 52 , 1, 54, 92, 20, 49, 91, 71, 87, 83, 18, 36, 30 , 6, 68, 15, 58, 86, 29 , 4, 84, 2, 100, 61, 12, 97, 98, 13, 69, 63, 14, 80, 94, 44, 74, 2, 46, 78, 77, 73, 3, 26, 38, 60, 31, 62, 33, 89, 17, 8, 95, 90, 22, 93, 75, 81, 19, 57, 39, 27, 32, 85, 42, 7, 11};
	std::vector<uint8_t> valid = {100, 26, 99, 116, 102, 123, 104, 101, 121, 95, 110, 111, 119, 95, 117, 95, 104, 97, 99, 107, 101, 100, 95, 109, 121, 95, 97, 108, 108, 115, 116, 97, 114, 125};
	std::cout << std::accumulate(valid.begin(), valid.end(), 0) << std::endl;
	std::srand(std::accumulate(valid.begin(), valid.end(), 0) ^ std::accumulate(mod.begin(), mod.end(), 0));
	for (unsigned i = 0; i < mod.size(); i++) {
		uint8_t tmp;
		size_t ind_a = std::rand() % mod.size();
		size_t ind_b = std::rand() % mod.size();
		tmp = mod[ind_a];
		mod[ind_a] = mod[ind_b];
		mod[ind_b] = tmp;
	}
	for (unsigned i = 0; i < valid.size(); i++) {
		//std::cout << "MOD: " << int(mod[i]) << std::endl;
		std::cout << valid[i] - mod[i] << ", ";
	}
}
