#include <algorithm>
#include <fstream>
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
	std::vector<int> list_1;
	std::vector<int> list_2;
	{
		int number;
		while (file >> number) {
			list_1.push_back(number);
			file >> number;
			list_2.push_back(number);
		}
	}
	
	std::sort(list_1.begin(), list_1.end());
	std::sort(list_2.begin(), list_2.end());

	int total_dist = 0;
	for (int i = 0; i < list_1.size(); i++) {
		cout << "comparing " << list_1[i] << "  " << list_2[i] << endl;
		total_dist += abs(list_1[i] - list_2[i]);
	}
	cout << "Total dist is " << total_dist << endl;

	return 1;
}