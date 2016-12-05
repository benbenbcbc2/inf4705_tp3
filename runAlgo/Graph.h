#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <algorithm>
#include <exception>
#include <unordered_map>
#include <unordered_set>

#include "filter_iterator.h"

class GroupExcept: public std::exception
{
public:
	virtual const char* what() const throw()
	{
		return "Not enough colors to color graph";
	}
};

enum Set {SEP, WSP, WTG};

template <class T>
struct Node {
	T id;
	int people;
	std::unordered_set<T> separate;
	std::unordered_set<T> want_sep;
	std::unordered_set<T> want_tog;

	Node(T id_, int people_) : id(id_), people(people_) {}
	
	bool connect(T other, Set s=SEP) { 
		auto *set = &this->separate;
		switch(s) {
		case WSP:
			set = &this->want_sep;
			break;
		case WTG:
			set = &this->want_tog;
			break;
		default: ;
		};
		if (set->find(other) != set->end())
			return false;
		set->insert(other);
		return true;
	}

	unsigned int degree() const {
		return separate.size();
	}
};

template <class T>
struct NodeGroup {
	NodeGroup(float average) {this->average = average;}
	float average;
	int people = 0;
	std::unordered_set<T> ids;
	std::unordered_set<T> separate;
	std::unordered_multiset<T> want_sep;
	std::unordered_multiset<T> want_tog;
	std::vector<Node<T>*> nodes;

	void insert(Node<T> *node) {
		ids.insert(node->id);
		nodes.push_back(node);
		people += node->people;
		separate.insert(node->separate.begin(),
				node->separate.end());
		want_sep.insert(node->want_sep.begin(),
				node->want_sep.end());
		want_tog.insert(node->want_tog.begin(),
				node->want_tog.end());
	}

	int cost(const Node<T> &node) const {
		return (want_sep.count(node.id) - want_tog.count(node.id)
			+ std::max(people - average + node.people,
				   0.0f));
	}

	bool can_insert(T id) const {
		return separate.find(id) == separate.end();
	}

	NodeGroup<T> split() {
		NodeGroup<T> other(average);
		int people_in = people;
		while (people > people_in/2) {
			Node<T> *disp = nodes.front();
			other.insert(disp);
			remove(disp);
		}
		recalc();
		return other;
	}
	
private:
	void remove(const Node<T> *node) {
		nodes.erase(std::find(nodes.begin(), nodes.end(), node));
		ids.erase(ids.find(node->id));
		people -= node->people;
	}

	void recalc() {
		separate.clear();
		want_sep.clear();
		want_tog.clear();
		for (auto node: nodes) {
		separate.insert(node->separate.begin(),
				node->separate.end());
		want_sep.insert(node->want_sep.begin(),
				node->want_sep.end());
		want_tog.insert(node->want_tog.begin(),
				node->want_tog.end());			
		}
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
	
	Node<T>* add_node(T id, int people) {
		auto it = nodes.find(id);
		if (it == nodes.end()) {
			Node<T> *node = new Node<T>(id, people);
			nodes.insert({id, node});
			node_vec.push_back(node);
			return node;
		}
		return it->second;
	}

	Node<T>* find_node(T id) {
		auto it = nodes.find(id);
		if (it == nodes.end()) {
			return nullptr;
		}
		return it->second;
	}

	bool connect(T id1, T id2, Set s = Set::SEP) {
		Node<T> *n1 = this->find_node(id1),
			*n2 = this->find_node(id2);
		bool res = false;
		res |= n1->connect(id2, s);
		res |= n2->connect(id1, s);
		return res;
	}

	static void print_all(std::unordered_set<T> l) {
		std::cout << "[";
		for (auto e: l) {
			std::cout << e << ", ";
		}
		std::cout << "]";
	}

	static void print_cur(const NodeGroup<T> &current) {
		print_all(current.ids);
		std::cout << " ";
		print_all(current.separate);
		std::cout << std::endl;
	}

	std::vector<std::unordered_set<T>> place(
		float average,
		unsigned int max_groups) {
		std::vector<Node<T>*> sorted_nodes(node_vec);
		std::sort(sorted_nodes.begin(),
			  sorted_nodes.end(),
			  [](Node<T>* a, Node<T>* b){
				  return a->degree() > b->degree();
			  });

		std::vector<NodeGroup<T>> groups;
		std::vector<std::unordered_set<T>> ret_groups;
		for (const auto &node: sorted_nodes) {
			auto fit = filter_iterator<typename std::vector<NodeGroup<T>>::iterator, std::function<bool(const NodeGroup<T>&)>>(
				groups.begin(),
				groups.end(),
				[node](const NodeGroup<T> &group) {
					return group.can_insert(node->id);
				});
			fit = std::max_element(
				fit, fit.end(),
				[node](const NodeGroup<T>& a,
				       const NodeGroup<T>& b){
					return (a.cost(*node) <
						b.cost(*node));
				});
			if (fit == fit.end()) {
				if (max_groups &&
				    groups.size() == max_groups)
					throw GroupExcept();
				groups.emplace_back(average);
				groups.back().insert(node);
				print_cur(groups.back()); 
			} else {
				fit->insert(node);
				print_cur(*fit); 
			}
		}

		while (groups.size() < max_groups) {
			auto it = std::max_element(
				groups.begin(), groups.end(),
				[](const NodeGroup<T> &a,
				   const NodeGroup<T> &b) {
					return a.people < b.people;
				});
			groups.push_back(it->split());
		}

		for (auto &group: groups) {
			ret_groups.push_back(group.ids);
		}

		for (auto &c: ret_groups) {
			print_all(c);
			std::cout << std::endl;
		}
		
		return ret_groups;
	}
};

#endif // _GRAPH_H_
