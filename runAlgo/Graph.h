#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <algorithm>
#include <exception>
#include <random>
#include <unordered_map>
#include <unordered_set>

#include "unordered_multiset.h"
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
	int people = 0, sep_score = 0, tog_score = 0;
	std::unordered_set<T> ids;
	jab::unordered_multiset<T> separate;
	jab::unordered_multiset<T> want_sep;
	jab::unordered_multiset<T> want_tog;
	std::vector<Node<T>*> nodes;

	void insert(Node<T> *node) {
		ids.insert(node->id);
		nodes.push_back(node);
		people += node->people;
		sep_score += want_sep.count(node->id);
		tog_score += want_tog.count(node->id);
		separate.insert(node->separate.begin(),
				node->separate.end());
		want_sep.insert(node->want_sep.begin(),
				node->want_sep.end());
		want_tog.insert(node->want_tog.begin(),
				node->want_tog.end());
	}

	float cost() const {
		return sep_score - tog_score + people - average;
	}

	float cost(const Node<T> &node) const {
		return (want_sep.count(node.id) - want_tog.count(node.id)
			+ std::max(people - average + node.people,
				   0.0f));
	}

	bool can_insert(T id) const {
		return !separate.has(id);
	}

	NodeGroup<T> split() {
		NodeGroup<T> other(average);
		int people_in = people;
		while (people > people_in/2) {
			Node<T> *disp = nodes.front();
			other.insert(disp);
			remove(disp);
		}
		return other;
	}
	
	void remove(const Node<T> *node) {
		nodes.erase(std::find(nodes.begin(), nodes.end(), node));
		ids.erase(ids.find(node->id));
		people -= node->people;
		sep_score -= want_sep.count(node->id);
		tog_score -= want_tog.count(node->id);
		separate.remove(node->separate.begin(),
				node->separate.end());
		want_sep.remove(node->want_sep.begin(),
				node->want_sep.end());
		want_tog.remove(node->want_tog.begin(),
				node->want_tog.end()); 
	}

	Node<T> *select() {
		std::uniform_int_distribution<> dis(0, nodes.size()-1);
		std::random_device rd;
		return nodes.at(dis(rd));
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

	static void print_all2(std::unordered_map<T, unsigned int> l) {
		std::cout << "[";
		for (auto e: l) {
			std::cout << e.first << ", ";
		}
		std::cout << "]";
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
		print_all2(current.separate.map());
		std::cout << std::endl;
	}

	void place(float average,
		   unsigned int max_groups,
		   std::function<void(std::vector<NodeGroup<T>>&)> cb) {
		std::vector<Node<T>*> sorted_nodes(node_vec);
		std::sort(sorted_nodes.begin(),
			  sorted_nodes.end(),
			  [](Node<T>* a, Node<T>* b){
				  return a->degree() > b->degree();
			  });

		std::vector<NodeGroup<T>> groups;
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
			} else {
				fit->insert(node);
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

		// Refine solution
		for(;;) {
			std::sort(groups.begin(),
				  groups.end(),
				  [](const NodeGroup<T> &a,
				     const NodeGroup<T> &b){
					  return a.cost() > b.cost();
				  });

			NodeGroup<T> &from = groups.front();
			Node<T> *node = from.select();
			from.remove(node);
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
			fit->insert(node);
			
			cb(groups);
		}
	}
};

#endif // _GRAPH_H_
