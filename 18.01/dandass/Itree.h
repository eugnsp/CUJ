#ifndef ITREE_H_
#define ITREE_H_

#include <vector>
#include <set>
#include <assert.h>
#include <algorithm>
#include <iterator>

template<class T, class A = std::allocator<T> > class itree 
{
public:
  typedef T                                 invl_type;
  typedef T::range_type                     range_type;
  typedef std::vector<invl_type, A>         invl_vec;
  typedef invl_vec::size_type               size_type;
  typedef invl_vec::reference               reference;
  typedef invl_vec::const_reference         const_reference;
  typedef invl_vec::value_type              value_type;    
  typedef invl_vec::iterator                iterator;
  typedef invl_vec::const_iterator          const_iterator;    
  typedef invl_vec::reverse_iterator        reverse_iterator;
  typedef invl_vec::const_reverse_iterator  
                                         const_reverse_iterator;

private:
  typedef struct node_tag       // Interval tree nodes
  {
    range_type      discrim;  // discriminant
    int             start;    // starting offset in AL and DH
    int             size;     // number of entris in AL and DH
    struct node_tag *left;    // left subtree
    struct node_tag *right;   // right subtree
  } node;

public:
    class query_iterator : 
        public std::iterator<std::forward_iterator_tag, itree>
    {
    public:
      query_iterator(void) : cur_node(NULL), index(0), 
                             p_al(NULL), p_dh(NULL) {}

      query_iterator(query_iterator& it) : 
        cur_node(it.cur_node), index(it.index), value(it.value),
        p_al(it.p_al), p_dh(it.p_dh) {}

      query_iterator& operator++(void) {// Prefix increment.
        Increment(); return *this; 
      }

      query_iterator operator++(int) {  // Postfix increment.
        query_iterator it(*this); 
        Increment(); return it; 
      }

      const T& operator*() const { 
        if (cur_node->discrim >= value)
          return *(*p_al)[cur_node->start + index];
        else
          return *(*p_dh)[cur_node->start + index];
      }

      const T* operator->() const { 
        if (cur_node->discrim >= value)
          return (*p_al)[cur_node->start + index]; 
        else
          return (*p_dh)[cur_node->start + index]; 
      }

      bool operator!=(query_iterator& it) const { 
        return !(*this == it); 
      }

      bool operator==(query_iterator& it) const { 
        return (value == it.value) && 
               (cur_node == it.cur_node) && (index == it.index);
      }

    private:
      node*                   cur_node;
      int                     index;
      std::vector<invl_type*> *p_al;
      std::vector<invl_type*> *p_dh;
      range_type              value;

      query_iterator(node* cur_node, int index, 
                     range_type value,
                     std::vector<invl_type*> *p_al, 
                     std::vector<invl_type*> *p_dh) : 
        cur_node(cur_node), index(index), value(value), 
        p_al(p_al), p_dh(p_dh) {}

      void Increment(void) {
        index++;
        if (index == cur_node->size) {
          if (cur_node->discrim == value) { // finished!
            cur_node = NULL;
            index = 0;
            return;
          }
          else if (cur_node->discrim > value)
            cur_node = cur_node->left;
          else
            cur_node = cur_node->right;

          init_node();
          return;
        }
        if (cur_node->discrim > value) {
          if ((*p_al)[cur_node->start + index]->low() <= value)
            return;
          else
            cur_node = cur_node->left;
        }
        else if (cur_node->discrim < value) {
          if ((*p_dh)[cur_node->start + index]->high() >= value)
            return;
          else
            cur_node = cur_node->right;
        }
        else  //(cur_node->discrim == value)
          return;
        init_node();
        return;
      }

      void init_node(void) {
        index = 0;        
        while (cur_node != NULL) {
          if (value < cur_node->discrim) {
            if ((cur_node->size != 0) &&
                ((*p_al)[cur_node->start]->low() <= value))
              return;
            else
              cur_node = cur_node->left;
          }
          else if (value > cur_node->discrim) {
            if ((cur_node->size != 0) && 
                ((*p_dh)[cur_node->start]->high() >= value))
              return;
            else
              cur_node = cur_node->right;
          }
          else //(value == cur_node->discrim)
            return;
        }
      }

      friend itree;
    };    // end of query_iterator declaration

  // constructors
  itree() : root(NULL) {}

  ~itree() { 
    deconstruct(); 
  }

  // functions forwarded to std::vector for processsing
  void reserve(size_type n) {
    assert(!constructed()); invls.reserve(n); 
  }

  size_type capacity() const { return invls.capacity(); }

  iterator begin() { 
    assert(!constructed()); return invls.begin(); 
  }

  const_iterator begin() const { return invls.begin(); }

  iterator end() { 
    assert(!constructed()); return invls.end(); 
  }

  const_iterator end() const { return invls.end(); }

  reverse_iterator rbegin() {
    assert(!constructed()); return invls.rbegin();
  }

  const_reverse_iterator rbegin() const {
    return invls.rbegin(); 
  }

  reverse_iterator rend() {
    assert(!constructed()); invls.rend(); 
  }

  const_reverse_iterator rend() const { return invls.rend(); }

  void resize(size_type n, T x = T()) {
    assert(!constructed()); invls.resize(n, x);
  }

  size_type size() const { return invls.size(); }

  size_type max_size() const { return invls.max_size(); }

  bool empty() const { return invls.empty(); }

  A get_allocator() const { return invls.get_allocator(); }

  reference at(size_type pos) {
    assert(!constructed()); return invls.at(pos); 
  }

  const_reference at(size_type pos) const { 
    return invls.at(pos); 
  }

  reference operator[](size_type pos) {
    assert(!constructed()); return invls[pos];
  }

  const_reference operator[](size_type pos) const { 
    return invls[pos]; 
  }

  reference front() {
    assert(!constructed()); return invls.front();
  }

  const_reference front() const { return invls.front(); }

  reference back() {
    assert(!constructed()); return invls.back(); }

  const_reference back() const { return invls.back(); }

  void push_back(const invl_type& x) { 
    assert(!constructed()); invls.push_back(x); 
  }

  void pop_back(void) { 
    assert(!constructed()); invls.pop_back(); 
  }

  void assign(const_iterator first, const_iterator last) {
    assert(!constructed()); invls.assign(first, last); 
  }

  void assign(size_type n, const T& x = T()) {
    assert(!constructed()); invls.assign(n, x);
  }

  iterator insert(iterator it, const T& x = T()) {
    assert(!constructed()); return invls.inserf(it, x);
  }

  void insert(iterator it, size_type n, const T& x) {
    assert(!constructed()); invls.insert(it, n, x);
  }

  void insert(iterator it, const_iterator first, 
              const_iterator last) {
    assert(!constructed()); invls.insert(it, fisrt, last);
  }

  iterator erase(iterator it) {
    assert(!constructed()); return invls.erase(it);
  }

  iterator erase(iterator first, iterator last) {
    assert(!constructed()); return invls.erase(first, last);
  }

  void clear() {
    assert(!constructed()); invls.clear();
  }

  void swap(itree x) {
    assert(!constructed()); invls.swap(x.invls);
  }

  // reverts container to initialization mode
  void deconstruct(void) { 
    delete_tree(root);  root = NULL; 
  }

  // recursively delete all the nodes in a subtree
  void delete_tree(node *cur_node) {
    if (cur_node != NULL) {
      delete_tree(cur_node->left);
      delete_tree(cur_node->right);
      delete cur_node;
    }
  }

  bool constructed() const { return root != NULL; }

  // build the interval tree structure and
  // put the container into query mode
  itree const& construct(void) {
    std::vector<range_type> values;
    std::vector<bool>       flags(invls.size(), false);
    int                     num_al_dh = 0;

    extract_values(values);   
    al.resize(invls.size());
    dh.resize(invls.size());
    root = construct_tree(values, flags, num_al_dh, 0, 
                          values.size() - 1);
    return *this;
  }

  // extract the sorted set of endpoint values
  void extract_values(std::vector<range_type>& values) {
    std::set<range_type>            value_set;
    std::set<range_type>::iterator  set_iter;
    invl_vec::const_iterator        invl_iter;

    for (invl_iter = invls.begin(); invl_iter != invls.end(); 
         invl_iter++) {
      value_set.insert(invl_iter->low());
      value_set.insert(invl_iter->high());
    }

    for (set_iter = value_set.begin(); 
         set_iter != value_set.end(); set_iter++)
      values.push_back(*set_iter);

    std::sort(values.begin(), values.end());
  }

  // recursively construct the tree
  node* construct_tree(const std::vector<range_type>& values, 
                       std::vector<bool>& flags, int& num_al_dh,
                       int start, int end) {
    int         discrim_pos;
    range_type  discrim;
    int         list_start;
    int         list_size;
    node        *root = NULL;
    node        *left = NULL;
    node        *right = NULL;
    bool        continue_left = false;
    bool        continue_right = false;

    if (start > end)
      return root;
    discrim_pos = (start + end) / 2;
    discrim = values[discrim_pos];
 
    list_start = num_al_dh;
    list_size = 0;
    for (int i = 0; i < invls.size(); i++) {
      if (flags[i])
        continue;

      if ((invls[i].low() <= discrim) && 
          (invls[i].high() >= discrim)) {
        al[num_al_dh] = &(invls[i]);
        dh[num_al_dh] = &(invls[i]);

        num_al_dh++;
        list_size++;
        flags[i] = true;
      }

      if ((invls[i].low() < discrim) && 
          (invls[i].high() < discrim))
        continue_left = true;

      if ((invls[i].low() > discrim) && 
          (invls[i].high() > discrim))
        continue_right = true;
    }

    // see if left and/or right subtree needs to be built
    if (continue_left && (start <= (discrim_pos - 1)))
      left = construct_tree(values, flags, num_al_dh, start, 
                            discrim_pos - 1);

    if (continue_right && ((discrim_pos + 1) <= end))
      right = construct_tree(values, flags, num_al_dh, 
                             discrim_pos + 1, end);

    // this node is needed only if there are entries in the 
    // AL/DH list or the left or right subtree exists.
    if ((list_size > 0) || (left != NULL) || (right != NULL)) {
      std::sort(&(al[list_start]), 
                &(al[list_start + list_size]), comp_for_al);
      std::sort(&(dh[list_start]), 
                &(dh[list_start + list_size]), comp_for_dh);

      root = new node;
      root->left = left;
      root->right = right;
      root->discrim = discrim;
      root->start = list_start;
      root->size = list_size;
    }
    return root;
  }

  // comparison function for sorting AL (ascending low)
  static bool comp_for_al(invl_type* c1, invl_type* c2) { 
    return c1->low() < c2->low(); 
  }

  // comparison function for sorting DH (descending high)
  static bool comp_for_dh(invl_type* c1, invl_type *c2) { 
    return c1->high() > c2->high(); 
  }

  query_iterator qbegin(range_type x) { 
    assert(constructed());
    query_iterator it(root, 0, x, &al, &dh); 
    it.init_node(); return it; 
  }

  query_iterator qend(range_type x) { 
    assert(constructed());
    query_iterator  it(NULL, 0, x, NULL, NULL); 
    return it; 
  }

private:
  invl_vec                invls;  // vector of intervals
  std::vector<invl_type*> al, dh; // vectors of interval ptrs
  node                    *root;  // Interval tree root node
};

#endif    // ITREE_H_
