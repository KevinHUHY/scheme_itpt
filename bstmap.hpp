using namespace std;
template <class Key, class T>

class bstmap
{
public:
  typedef Key                key_type;
  typedef T                  data_type;
  typedef std::pair<const Key, T> value_type;
  typedef unsigned int       size_type;
  typedef int                difference_type;

private:
  typedef bstmap<Key, T> Self;
  class Node {
  public:
    Node* parent_m;
    Node* left_m;
    Node* right_m;
    value_type val_m;

    Node(){};
    Node(Node* parent, Node* left, Node* right, value_type val)
    :parent_m(parent), left_m(left), right_m(right), val_m(val)
    {};
  };

private:
  Node* root_m;
  size_type size_m;

public:
  class iterator {
    friend class bstmap;
  // public:
  private:
    Node* pNode_m;
  public:
    iterator(){};
    
    iterator(Node* p)
    :pNode_m(p)
    {};

    Node* get_pointer()
    {
      return pNode_m;
    }

    value_type& operator*() const
    {
      if(pNode_m != 0) {
        return pNode_m -> val_m;
      } else {
        cerr << "improper dereference." << endl;
        exit(1);
      }
    }

    value_type* operator->() const
    {
      return &pNode_m ->val_m;
    }

    //prefix sum
    iterator& operator++()
    {
      if(pNode_m -> right_m != 0) {
        pNode_m = pNode_m -> right_m;
        while(pNode_m -> left_m != 0) {
          pNode_m = pNode_m -> left_m;
        }
      } else {
        Node* pa = pNode_m -> parent_m;
        while(pa != 0 && pNode_m != pa -> left_m) {
          pNode_m = pa;
          pa = pNode_m -> parent_m;
        }
        pNode_m = pa;
      }
      return *this;
    }
    
    iterator operator++(int)
    {
      iterator temp(pNode_m);
      operator++();
      return temp;
    }

    bool operator==(const iterator& it) const
    {
      return pNode_m == it.pNode_m;
    }

    bool operator!=(const iterator& it) const
    {
      return pNode_m != it.pNode_m;
    }

  };

  class const_iterator {
    friend class bstmap;
  // public:
  private:
    const Node* pNode_m;
  public:    
    const_iterator(Node* p)
    :pNode_m(p)
    {};

    const_iterator(iterator it)
    :pNode_m(it.get_pointer())
    {};

    const value_type& operator*() const
    {
      if(pNode_m != 0) {
        return pNode_m -> val_m;
      } else {
        cerr << "improper dereference." << endl;
        exit(1);
      }
    }

    const value_type* operator->() const
    {
      return &pNode_m ->val_m;
    }

    //prefix sum
    const_iterator& operator++()
    {
      if(pNode_m -> right_m != 0) {
        pNode_m = pNode_m -> right_m;
        while(pNode_m -> left_m != 0) {
          pNode_m = pNode_m -> left_m;
        }
      } else {
        Node* pa = pNode_m -> parent_m;
        while(pa != 0 && pNode_m != pa -> left_m) {
          pNode_m = pa;
          pa = pNode_m -> parent_m;
        }
        pNode_m = pa;
      }
      return *this;
    }
    
    const_iterator operator++(int)
    {
      const_iterator temp(pNode_m);
      operator++();
      return temp;
    }

    bool operator==(const const_iterator& it) const
    {
      return pNode_m == it.pNode_m;
    }

    bool operator!=(const const_iterator& it) const
    {
      return pNode_m != it.pNode_m;
    }

  };

public:
  // default constructor to create an empty map
  bstmap()
  :root_m(0), size_m(0)
  {}

  // overload copy constructor to do a deep copy
  Node* copy_tree(const Node* root)
  {
    if(root == 0) {
      return 0;
    } else {
      Node* ret = new Node(0, copy_tree(root -> left_m), 
                            copy_tree(root -> right_m), root ->val_m);
      if(ret -> left_m != 0) {
        ret -> left_m -> parent_m = ret;
      }
      if(ret -> right_m != 0) {
        ret -> right_m -> parent_m = ret;
      }
      return ret;
    }
  }

  bstmap(const Self& x) 
  {
    size_m = x.size_m;
    root_m = copy_tree(x.root_m);
  }

  // overload assignment to do a deep copy
  Self& operator=(const Self& x)
  {
    size_m = x.size_m;
    root_m = copy_tree(x.root_m);
    return *this;
  }

  // accessors:
  iterator begin()
  {
    Node* cur = root_m;
    if(cur == 0) {
      return iterator(0);
    }
    while(cur -> left_m != 0) {
      cur = cur -> left_m;
    }
    return iterator(cur);
  }

  const_iterator begin() const
  {
    Node* cur = root_m;
    if(cur == 0) {
      return const_iterator(0);
    }
    while(cur -> left_m != 0) {
      cur = cur -> left_m;
    }
    return const_iterator(cur);
  }
  
  iterator end()
  {
    return iterator(0);
  }

  const_iterator end() const
  {
    return const_iterator(0);
  }

  bool empty() const
  {
    return size_m == 0;
  }

  size_type size() const
  {
    return size_m;
  }

  // insert/erase
  std::pair<iterator,bool> insert(const value_type& x)
  {
    if(root_m == 0) {
      root_m = new Node(root_m, 0, 0, x);
      return std::pair<iterator, bool>(iterator(root_m), true);
    }
    Node* p = root_m;
    while(p != 0) {
      if(p -> val_m.first < x.first) {
        if(p -> right_m == 0) {
          p -> right_m = new Node(p, 0, 0, x);
          ++size_m;
          return std::pair<iterator, bool>(iterator(p -> right_m), true);
        } else {
          p = p -> right_m;
        }
      } else if(p -> val_m.first > x.first){
        if(p -> left_m == 0) {
          p -> left_m = new Node(p, 0, 0, x);
          ++size_m;
          return std::pair<iterator, bool>(iterator(p -> left_m), true);
        } else {
          p = p -> left_m;
        }
      } else {
        return std::pair<iterator, bool>(iterator(p), false);
      }
    }
    return std::pair<iterator, bool>(iterator(p), false);
  }

  void erase(iterator pos)
  {
    if(pos.pNode_m == 0) {
      return;
    }
    if(pos.pNode_m -> left_m == 0 && pos.pNode_m -> right_m == 0) {
      if(pos.pNode_m -> parent_m != 0) {
        if(pos.pNode_m -> parent_m -> left_m == pos.pNode_m) {
          pos.pNode_m -> parent_m -> left_m = 0;
        } else {
          pos.pNode_m -> parent_m -> right_m = 0;
        }
      } else {
        root_m = 0;
      }
    } else if(pos.pNode_m -> left_m == 0 || pos.pNode_m -> right_m == 0) {
      Node* child = 0;
      if(pos.pNode_m -> left_m == 0) {
        child = pos.pNode_m -> right_m;
      } else {
        child = pos.pNode_m -> left_m;
      }
      if(pos.pNode_m -> parent_m != 0) {
        if(pos.pNode_m -> parent_m -> right_m == pos.pNode_m) {
          pos.pNode_m -> parent_m -> right_m = child;
        } else {
          pos.pNode_m -> parent_m -> left_m = child;
        }
      } else {
        root_m = child;
      }
      child -> parent_m = pos.pNode_m -> parent_m;
    } else {
      Node* swapper = pos.pNode_m -> right_m;
      while(swapper -> left_m != 0) {
        swapper = swapper -> left_m;
      }
      if(swapper -> parent_m == pos.pNode_m) {
        swapper -> parent_m -> right_m = 0;
      } else {
        swapper -> parent_m -> left_m = 0;
      }
      swapper -> left_m = pos.pNode_m -> left_m;
      swapper -> right_m = pos.pNode_m -> right_m;
      swapper -> parent_m = pos.pNode_m -> parent_m;
      swapper -> left_m -> parent_m = swapper;
      swapper -> right_m -> parent_m = swapper;
      if(swapper -> parent_m == 0) {
        root_m = swapper;
      }
    }
    delete pos.pNode_m;
    return;
  }

 // iterator find(const Key& x);
  size_type erase(const Key& x)
  {
    iterator it = find(x);
    erase(it);
    return 1;
  }

  void delete_tree(Node* root)
  {
    if(root == 0) {
      return;
    }
    delete_tree(root -> left_m);
    delete_tree(root -> right_m);
    delete root;
  }
  
  void clear()
  {
    delete_tree(root_m);
    root_m = 0;
  }

  // map operations:
  iterator find(const Key& x)
  {
    Node* p = root_m;
    while(p != 0) {
      if(p -> val_m.first < x) {
        p = p -> right_m;
      } else if(p -> val_m.first > x) {
        p = p -> left_m;
      } else {
        return iterator(p);
      }
    }
    return end();
  }

  const_iterator find(const Key& x) const
  {
    Node* p = root_m;
    while(p != 0) {
      if(p -> val_m.first < x) {
        p = p -> right_m;
      } else if(p -> val_m.first > x) {
        p = p -> left_m;
      } else {
        return const_iterator(p);
      }
    }
    return end();
  }

  size_type count(const Key& x) const
  {
    return (find(x) != end());
  }

  T& operator[](const Key& k)
  {
    iterator it = find(k);
    if(it != end()) {
      return it -> second;
    } else {
      value_type new_pair = value_type(k, T());
      return insert(new_pair).first -> second;
    }
  }
};
