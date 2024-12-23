#include <algorithm>
#include <assert.h>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

using namespace std;

template<typename V, typename T>
bool std_contains(const vector<V>& vec, const T& value) {
	return std::find(vec.begin(), vec.end(), value) != vec.end();
}

template<typename V, typename T>
bool std_contains(const V& vec, const T& value) {
	return vec.find(value) != vec.end();

}
struct node {
	vector<string> connections;
};

unordered_map<string, node> g_graph;

void part_one() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	string line;
	while (file >> line) {
		const string node_1 = line.substr(0, 2);
		const string node_2 = line.substr(3);
		g_graph[node_1].connections.push_back(node_2);
		g_graph[node_2].connections.push_back(node_1);
	}

	unordered_set<string> g_connections;

	for (const auto& it : g_graph) {
		const auto& node_name = it.first;
		const auto& node_neighbors = it.second.connections;

		for (size_t i = 0; i < node_neighbors.size(); i++) {
			const auto& neighbor_name = node_neighbors[i];
			const auto& neighors_connections = g_graph[neighbor_name].connections;

			bool found = false;
			for (size_t j = 0; j < neighors_connections.size() && !found; j++) {
				const auto& neighbor_connection = neighors_connections[j];

				for (size_t l = i + 1; l < node_neighbors.size(); l++) {
					const auto& test_name = node_neighbors[l];
					if (node_name[0] != 't' && neighbor_name[0] != 't' && test_name[0] != 't') {
						continue;
					}

					if (std_contains(neighors_connections, test_name)) {
						vector<string> vecs = { node_name, neighbor_name, test_name };
						std::sort(vecs.begin(), vecs.end());
						g_connections.insert(vecs[0] + vecs[1] + vecs[2]);
						found = true;
					}
				}
			}
		}
	}

	for (auto it = g_connections.begin(); it != g_connections.end(); ++it) {
		cout << *it << endl;
	}
	cout << "Num found = " << g_connections.size() << endl;

}

void part_two() {
	ifstream file("input.txt");
	if (!file.is_open()) {
		return;
	}

	string line;

	while (file >> line) {
		const string node_1 = line.substr(0, 2);
		const string node_2 = line.substr(3);
		g_graph[node_1].connections.push_back(node_2);
		g_graph[node_2].connections.push_back(node_1);
	}

	uint64_t max_clique_size = 0;
	string password;

	for (const auto& itv : g_graph) {
		vector<string> clique = { itv .first};

		for (const auto& it: g_graph) {
			unordered_set<string> visited;
			bool found = true;
			for (int l = 0; l < clique.size(); l++) {
				if (!std_contains(g_graph[clique[l]].connections, it.first)) {
					found = false;
					break;
				}
			}
			if (found) {
				clique.push_back(it.first);
			}							
		}

		if (clique.size() <= max_clique_size) {
			continue;
		}
		max_clique_size = clique.size();

		std::sort(clique.begin(), clique.end());

		password.clear();
		for (int i = 0; i < clique.size(); i++) {
			password += clique[i];
			password += ',';

		/*	cout << clique[i];
			if (i < clique.size() - 1) {
				cout << ",";
			}*/
		}
		cout << endl;
	}

	cout << "Password is " << password << endl;
}
/**
 *
 */
int main() {
	cout << "Part one..........................................................\n";	
	part_one();

	cout << "\n\nPart one..........................................................\n";
	part_two();
	return 1;
}
