#include <unordered_map>
#include <forward_list>
#include <string>
#include <iostream>
#include <vector>
#include <iterator>
#include <sstream>
#include <stack>
#include <regex>

struct node {
	char name = {1};
	char previous = {1};
	std::forward_list<std::pair<char, int>> adj_list;
	int distance = 1000;
};

int TraverseGraph(std::unordered_map<char, node>& unvisited_nodes, char start, char end, int allowed_distance) {

	std::unordered_map<char, node> visited_nodes;

	if (unvisited_nodes.find(start) == unvisited_nodes.end()) { //find start node
		return -1;
	}
	node current_node = unvisited_nodes[start];
	current_node.distance = 0;

	while (!unvisited_nodes.empty()) { //dijkstra's algorithm
		int new_distance = 0;
		for (auto it = current_node.adj_list.begin(); it != current_node.adj_list.end(); ++it) {
			if (unvisited_nodes.find((*it).first) != unvisited_nodes.end()) {
				new_distance = current_node.distance + (*it).second;
				int& distance_to_adj = unvisited_nodes[(*it).first].distance;
				if (new_distance < distance_to_adj) {
					distance_to_adj = new_distance;
					unvisited_nodes[(*it).first].previous = current_node.name;
				}
			}
		}
		char prev_node_name = current_node.name;
		visited_nodes.emplace(current_node.name, current_node);
		current_node = visited_nodes[prev_node_name];
		unvisited_nodes.erase(prev_node_name);

		int shortest_dist = 1000;
		for (auto it = unvisited_nodes.begin(); it != unvisited_nodes.end(); ++it) { //get closest node
			if ((*it).second.distance < shortest_dist) {
				shortest_dist = (*it).second.distance;
				current_node = (*it).second;
			}
		}
	}

	std::stack<char> route;
	int distance_travelled = 0;

	char to_add = visited_nodes[end].name;
	while (visited_nodes[to_add].name != start) {

		//check if route is available		
		if (visited_nodes[to_add].distance >= allowed_distance) {
			const char error3[] = { "E3" };
			std::cout << error3 << std::endl;
			return -1;
		}
		route.push(to_add);
		to_add = visited_nodes[to_add].previous;
	}
	route.push(start);

	std::string output = "";
	while (!route.empty()) {
		output += route.top();
		route.pop();
		if (route.size() >= 1) {
			output += "->";
		}
	}
	std::cout << output << std::endl;
	return 0;
}

int main() {
	const char error1[] = { "E1" };
	const char error2[] = { "E2" };

	std::string line1;
    getline(std::cin, line1);
	if (!std::regex_match(line1, std::regex("(\\[[A-Z],[A-Z],[0-9]+\\]\\ )+\\[[A-Z],[A-Z],[0-9]+\\]"))) {
		std::cout << error1 << std::endl;
		return -1;
	}

	std::string line2;
	getline(std::cin, line2);
	if (!std::regex_match(line2, std::regex("[A-Z]->[A-Z],[0-9]+"))) {
		std::cout << error1 << std::endl;
		return -1;
	}

	std::istringstream iss(line1);
	std::vector<std::string> edges((std::istream_iterator<std::string>(iss)),
		std::istream_iterator<std::string>());

	std::unordered_map<char, node> graph;
	for (std::string& edge : edges) { //get edges
		std::stringstream s;
		s << edge.substr(5, (edge.size() - 6));
		int dist;
		s >> dist;

		if (graph.find(edge[1]) == graph.end()) { //create graph data structure
			node n;
			n.name = edge[1];
			n.adj_list.push_front(std::make_pair(edge[3], dist)); //fix these numbers
			graph.emplace(edge[1], n);
		}
		else{
			for (auto it = graph[edge[1]].adj_list.begin(); it != graph[edge[1]].adj_list.end(); ++it) { //check for duplicate edges
				if ((*it).first == edge[3]) {
					std::cout << error2 << std::endl;
					return -1;
				}
			}
			graph[edge[1]].adj_list.push_front(std::make_pair(edge[3], dist));
		}
		if (graph.find(edge[3]) == graph.end()) {
			node n;
			n.name = edge[3];
			n.adj_list.push_front(std::make_pair(edge[1], dist));
			graph.emplace(edge[3], n);
		}
		else{
			graph[edge[3]].adj_list.push_front(std::make_pair(edge[1], dist));
		}
	}

	std::stringstream s;
	s << line2.substr(5, (line2.size()-5));
	int max_dist;
	s >> max_dist;
	TraverseGraph(graph, line2[0], line2[3], max_dist);
	
	return 0;
}