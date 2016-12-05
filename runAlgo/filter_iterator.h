#ifndef _FILTER_ITERATOR_H_
#define _FILTER_ITERATOR_H_

#include <algorithm>

template<typename iterator_type, typename predicate_type>
  class filter_iterator
{
 public:
  typedef typename iterator_type::value_type value_type;

 filter_iterator(const iterator_type& begin,
		 const iterator_type& end,
		 const predicate_type& pred):
  current_(begin), end_(end), pred_(pred)
  {
    advance();
  }

 filter_iterator(const iterator_type& end):
  current_(end), end_(end), pred_({}) {}
  
  filter_iterator<iterator_type, predicate_type> end() const {
    return filter_iterator<iterator_type, predicate_type>(end_);
  }

  value_type& operator*() const { return *current_; }
  value_type* operator->() const { return &(*current_); }

  filter_iterator& operator++() { advance(); return *this; }

  bool operator==(const filter_iterator& rhs) const {
    return current_ == rhs.current_;
  }
  bool operator!=(const filter_iterator& rhs) const {
    return !(operator==(rhs));
  }

 private:
  void advance() {
      current_ = std::find_if(++current_, end_, pred_);
  }

  iterator_type current_;
  iterator_type end_;
  predicate_type pred_;
};

#endif // _FILTER_ITERATOR_H_
