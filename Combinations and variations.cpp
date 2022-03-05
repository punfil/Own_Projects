#include <iostream>
#include <vector>

void GeneratePermutations(std::vector<int> used_numbers, std::vector<int> not_used_numbers) {
	std::vector<int> used2, not_used2;
	for (int i = 0; i < not_used_numbers.size(); i++) {
		used2 = used_numbers;
		not_used2 = not_used_numbers;
		used2.emplace_back(not_used_numbers[i]);
		not_used2.erase(not_used2.begin()+i);
		GeneratePermutations(used2, not_used2);
	}
	if (!not_used_numbers.size()) {
		std::cout << "[ ";
		for (int i = 0; i < used_numbers.size(); i++) {
			std::cout << used_numbers[i] << " ";
		}
		std::cout << "]\n";
	}
}

void GenerateVariationsWithoutRepetitions(std::vector<int> used_numbers, std::vector<int> not_used_numbers, int depth) {
	std::vector<int> used2, not_used2;
	for (int i = 0; i < not_used_numbers.size() && depth>0; i++) {
		used2 = used_numbers;
		not_used2 = not_used_numbers;
		used2.emplace_back(not_used_numbers[i]);
		not_used2.erase(not_used2.begin() + i);
		GenerateVariationsWithoutRepetitions(used2, not_used2, depth-1);
	}
	if (depth==0) {
		if (!not_used_numbers.size()) {
			std::cout << "[ ";
			for (int i = 0; i < used_numbers.size(); i++) {
				std::cout << used_numbers[i] << " ";
			}
			std::cout << "]\n";
		}
		for (int k = 0;k<not_used_numbers.size();k++){
			std::cout << "[ ";
			for (int i = 0; i < used_numbers.size(); i++) {
				std::cout << used_numbers[i] << " ";
			}
			std::cout << "]\n";
		}
	}
}
void GenerateVariationsWithReperitions(std::vector<int> used_numbers, std::vector<int> not_used_numbers, int depth) {
	std::vector<int> used2, not_used2;
	for (int i = 0; i < not_used_numbers.size() && depth>0; i++) {
		used2 = used_numbers;
		used2.emplace_back(not_used_numbers[i]);
		not_used2 = not_used_numbers;
		GenerateVariationsWithReperitions(used2, not_used2, depth - 1);
	}
	if (depth == 0) {
		std::cout << "[ ";
		for (int i = 0; i < used_numbers.size(); i++) {
			std::cout << used_numbers[i] << " ";
		}
		std::cout << "]\n";
	}
}
void GenerateCombinationsWithoutRepetitions(std::vector<int> used_numbers, std::vector<int> not_used_numbers, int depth) {
	std::vector<int> used2, not_used2;
	for (int i = 0; i < not_used_numbers.size() && depth>0; i++) {
		if (used_numbers.size()==0 || used_numbers[used_numbers.size()-1] < not_used_numbers[i]){
			used2 = used_numbers;
			not_used2 = not_used_numbers;
			used2.emplace_back(not_used_numbers[i]);
			not_used2.erase(not_used2.begin() + i);
			GenerateCombinationsWithoutRepetitions(used2, not_used2, depth - 1);
		}
	}
	if (depth == 0) {
		if (!not_used_numbers.size()) {
			std::cout << "[ ";
			for (int i = 0; i < used_numbers.size(); i++) {
				std::cout << used_numbers[i] << " ";
			}
			std::cout << "]\n";
		}
		std::cout << "[ ";
		for (int i = 0; i < used_numbers.size(); i++) {
			std::cout << used_numbers[i] << " ";
		}
		std::cout << "]\n";
	}
}
int main() {
	unsigned a = 6;
	std::vector<int> liczby;
	std::vector<int> nie_uzyte;
	for (int i = 1; i < a+1; i++) {
		liczby.emplace_back(i);
	}
	GeneratePermutations(nie_uzyte, liczby);
	std::cout << "\nVariations without repetitions\n\n";
	GenerateVariationsWithoutRepetitions(nie_uzyte, liczby, 3);
	std::cout << "\nVariations with repetitions\n\n";
	GenerateVariationsWithReperitions(nie_uzyte, liczby, 3);
	std::cout << "\nCombinations without repetitions\n\n";
	GenerateCombinationsWithoutRepetitions(nie_uzyte, liczby, 4);
	return 0;
}