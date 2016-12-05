#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <algorithm>
#include <unordered_map>
#include <unordered_set>

template <class T>
struct Node {
	T id;
	std::unordered_set<Node<T>*> neighbors;

	Node(T id_) : id(id_) {}
	
	bool connect(Node<T> *other) {
		if (neighbors.find(other) == neighbors.end()) {
			neighbors.insert(other);
			return true;
		}
		return false;
	}

	unsigned int degree() {
		return neighbors.size();
	}
};


template <class T>
class Graph {
	std::unordered_map<T, Node<T>*> nodes;
	std::vector<Node<T>*> node_vec;
public:
	~Graph() {
		for (auto node : node_vec) {
			delete node;
		}
	}
	
	Node<T>* add_node(T id) {
		auto it = nodes.find(id);
		if (it == nodes.end()) {
			Node<T> *node = new Node<T>(id);
			nodes.insert({id, node});
			node_vec.push_back(node);
			return node;
		}
		return it->second;
	}

	bool connect(T id1, T id2) {
		Node<T> *n1 = this->add_node(id1),
			*n2 = this->add_node(id2);
		bool res = false;
		res |= n1->connect(n2);
		res |= n2->connect(n1);
		return res;
	}

	static void print_all(std::unordered_set<T> l) {
		std::cout << "[";
		for (auto e: l) {
			std::cout << e << ", ";
		}
		std::cout << "]";
	}

	static void print_cur(std::pair<std::unordered_set<T>,
			      std::unordered_set<T>> current) {
		print_all(current.first);
		std::cout << " ";
		print_all(current.second);
		std::cout << std::endl;
	}

	std::pair<bool, std::vector<std::unordered_set<T>>>
		color(unsigned int max_colors=0) {
		std::vector<Node<T>*> sorted_nodes(node_vec);
		std::sort(sorted_nodes.begin(),
			  sorted_nodes.end(),
			  [](Node<T>* a, Node<T>* b){
				  return a->degree() > b->degree();
			  });

		std::vector<std::pair<std::unordered_set<T>,
			std::unordered_set<T>>> colors;
		std::vector<std::unordered_set<T>> ret_colors;
		for (const auto &node: sorted_nodes) {
			auto it = std::find_if(
				colors.begin(),
				colors.end(),
				[node](std::pair<std::unordered_set<T>,
				       std::unordered_set<T>> col) {
					auto &n = col.second;
					return (n.find(node->id) ==
						n.end());
				});
			if (it == colors.end()) {
				if (max_colors &&
				    colors.size() == max_colors)
					goto break_out;
				it = colors.insert(it, {{}, {}});
			}
			it->first.insert(node->id);
			for (auto n : node->neighbors) {
				it->second.insert(n->id);
			}
		}

		for (auto &col: colors) {
			ret_colors.push_back(col.first);
		}

		for (auto &c: ret_colors) {
			print_all(c);
			std::cout << std::endl;
		}
		
		return {true, ret_colors};
	break_out:
		return {false, std::vector<std::unordered_set<T>>()};
	}
};

#endif // _GRAPH_H_
