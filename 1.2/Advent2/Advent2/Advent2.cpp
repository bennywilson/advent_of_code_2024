#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	string line;
	std::vector<int> list1;
	std::unordered_map<int, int> list2_mappings;

	int number;
	while (file >> number) {
		list1.push_back(number);
		file >> number;
		list2_mappings[number]++;
	}

	int similarity_score = 0;
	for (int i = 0; i < list1.size(); i++) {
		const int cur_num = list1[i];
		const int num_repeats = list2_mappings[cur_num];
		similarity_score += cur_num * num_repeats;
	}
	cout << "Similarity score is " << similarity_score << endl;

	return 1;
}