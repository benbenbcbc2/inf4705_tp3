#ifndef _UNORDERED_MULTISET_H_
#define _UNORDERED_MULTISET_H_

namespace jab {
  template<class K>
  class unordered_multiset {
    std::unordered_map<K, unsigned int> multiplicities;
  public:
    unsigned int count(K key) const {
      try {
	return multiplicities.at(key);
      } catch (std::out_of_range &e) {
	return 0;
      }
    }

    bool has(K key) const {
      return count(key);
    }

    template<class Iter>
      void insert(Iter begin, Iter end) {
      for (Iter i = begin; i != end; ++i) {
	insert(*i);
      }
    }
    
    void insert(K key) {
      try {
	multiplicities.at(key) += 1;
      } catch (std::out_of_range &e) {
	multiplicities.insert({key, 1});
      }
    }

    template<class Iter>
      void remove(Iter begin, Iter end) {
      for (Iter i = begin; i != end; ++i) {
	remove(*i);
      }
    }

    void remove(K key) {
      unsigned int m = 1;
      m = multiplicities.at(key) -= 1;
      if (!m)
	multiplicities.erase(key);
    }

    std::unordered_map<K, unsigned int> map() const {
      return multiplicities;
    }
  };
}

#endif // _UNORDERED_MULTISET_H_
