#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// 7198
int main() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	// Collect page ordering rules
	unordered_map<int, unordered_set<int>> page_to_following;
	string line;
	while (std::getline(file, line)) {
		if (line.length() == 0) {
			break;
		}
		std::stringstream ss(line);

		int earlier_page;
		char divider;
		int later_page;
		while(ss >> earlier_page) {
			ss >> divider;
			ss >> later_page;
			cout << earlier_page << divider << later_page << endl;
			page_to_following[earlier_page].insert(later_page);
		}
	}

	// Process updates
	int total_middle_pages = 0;
	vector<int> update_pages;
	while (std::getline(file, line)) {
		update_pages.resize(0);
		int num;
		std::stringstream ss(line);
		while (ss >> num) {
			char divider;
			ss >> divider;
			update_pages.push_back(num);
			cout << num << " ";
		}
		bool in_order = true;
		for (int i = 1; i < update_pages.size(); i++) {
			const int prev = update_pages[i-1];
			const int next = update_pages[i];
			if (page_to_following[prev].find(next) == page_to_following[prev].end()) {
				in_order = false;
				break;
			}
		}

		if (in_order) {
			total_middle_pages += update_pages[update_pages.size() / 2];
		}
		cout << endl;
	}

	cout << "Total middle pages is " << total_middle_pages;
}
