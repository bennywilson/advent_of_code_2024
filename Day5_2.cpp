#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

unordered_map<int, unordered_set<int>> page_to_following;

int main() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return 0;
	}

	// Collect page ordering rules
	string line;
	char divider;
	while (std::getline(file, line)) {
		if (line.length() == 0) {
			break;
		}

		std::stringstream ss(line);
		int earlier_page;
		int later_page;
		while (ss >> earlier_page) {
			ss >> divider;
			ss >> later_page;
			unordered_set<int>& following_pages = page_to_following[earlier_page];
			following_pages.insert(later_page);
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
			ss >> divider;
			update_pages.push_back(num);
		}

		bool in_order = true;
		for (size_t i = 1; i < update_pages.size(); i++) {
			const int prev = update_pages[i - 1];
			const int next = update_pages[i];
			const unordered_set<int>& following_pages = page_to_following[prev];
			if (following_pages.find(next) == following_pages.end()) {
				in_order = false;
				break;
			}
		}

		if (in_order) {
			continue;
		}

		struct page_sorter {
			inline bool operator() (const int a, const int b) {
				const unordered_set<int>& following_pages = page_to_following[a];
				return following_pages.find(b) != following_pages.end();
			}
		};
		std::sort(update_pages.begin(), update_pages.end(), page_sorter());
		total_middle_pages += update_pages[update_pages.size() / 2];
	}

	cout << "Total middle pages is " << total_middle_pages;

	return 1;
}
