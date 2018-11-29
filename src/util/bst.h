/****************************************************************************
  FileName     [ bst.h ]
  PackageName  [ util ]
  Synopsis     [ Define binary search tree package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
 ****************************************************************************/

#ifndef BST_H
#define BST_H

#include <cassert>

using namespace std;

template <class T> class BSTree;

// BSTreeNode is supposed to be a private class. User don't need to see it.
// Only BSTree and BSTree::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class BSTreeNode
{
  // TODO: design your own class!!
  private:
    friend class BSTree<T>;
    friend class BSTree<T>::iterator;
    BSTreeNode( const T& t, BSTreeNode<T>* ptrP = nullptr, 
        BSTreeNode<T>* ptrR = nullptr,
        BSTreeNode<T>* ptrL = nullptr):
      _data(t), _color(0), _parent(ptrP), 
      _child_R(ptrR), _child_L(ptrL){}
    T                 _data;
    char              _color;
    BSTreeNode<T>*    _parent;
    BSTreeNode<T>*    _child_R;
    BSTreeNode<T>*    _child_L;
};


template <class T>
class BSTree
{
  public:
    // TODO: design your own class!!
    BSTree(): _root( nullptr ), _size(0) {}
    class iterator {
      public:
        friend class BSTree;
        const T&  operator *   () const             ;
        T&        operator *   ()                   ;
        iterator& operator ++  ()                   ;
        iterator  operator ++  (int)                ;
        iterator& operator --  ()                   ;
        iterator  operator --  (int)                ;
        iterator& operator = (const iterator& i)    ;
        bool operator != (const iterator& i) const  ;
        bool operator == (const iterator& i) const  ;
      private:
        BSTreeNode<T>* ptr;
    };
    void    pop_front();
    void    pop_back();
    void    clear();
    void    sort() const;
    size_t  size() const;
    bool    empty() const;
    bool    erase( const T& );
    // just erase first encountered.
    // return false iff not found.
    bool    erase( iterator pos );
    // pos would be valid except when ADT.empty() in this homework.
    // pos invalid iff ADT is empty, return false in this case.
    iterator find(const T&);
    iterator end()   const;
    iterator begin() const;

    void insert( const T& );
    void print() const;
  private:
    static const int RED   = 0;
    static const int BLACK = 1;
    BSTreeNode<T>* _root;
    void insert_fix( BSTreeNode<T>* );
    void delete_fix( BSTreeNode<T>*, BSTreeNode<T>*, bool);
    // false mean _child_R;
    // true  mean _child_L;
    void right_rot ( BSTreeNode<T>* );
    void left__rot ( BSTreeNode<T>* );
    size_t _size;
};

template<typename T>
void
BSTree<T>::insert( const T& other ){
  if( empty() ){
    _root = new BSTreeNode<T>( other );
    _root -> _color = BLACK;
  }else{
    bool done = false;
    auto* ptr = _root;
    while( !done ){
      if( other > ptr->_data ){
        if( ptr->_child_R == nullptr ){
          ptr->_child_R = new BSTreeNode<T>( other, ptr);
          done = true;
        }else{
          ptr = ptr->_child_R;
          continue;
        }
      }else{
        if( ptr->_child_L == nullptr ){
          ptr->_child_L = new BSTreeNode<T>( other, ptr);
          done = true;
        }else{
          ptr = ptr->_child_L;
          continue;
        }
      }
    }
    insert_fix( ptr );
  }
  _size += 1;
}

template<typename T>
void
BSTree<T>::insert_fix( BSTreeNode<T>* ptr ){
  // ptr must have RED;
  if( ptr == _root ){
    assert( 0 && "WTF, insert_fix met root??" );
  }else if( ptr -> _parent == _root ){
    return;
  }

  // ptr must have grandparent.
  while( ptr->_parent->_color == RED ){
    auto* GParent_ptr = ptr->_parent->_parent;
    auto* uncle_ptr   = GParent_ptr;
    if( ptr->_parent == GParent_ptr->_child_L ){
      // case I, parent of ptr is a left-child.
      uncle_ptr = GParent_ptr->_child_R;
      // check uncle color.
      if( uncle_ptr != nullptr ){
        if( uncle_ptr -> _color == RED ){
          GParent_ptr->_color  = RED;
          uncle_ptr->_color    = BLACK;
          ptr->_parent->_color = BLACK;
          ptr = GParent_ptr;
          continue;
        }
      }
      // uncle is (nil == nullptr) or BLACK
      if( ptr == ptr->_parent->_child_R ){
        // inner-node.
        ptr = ptr->_parent;
        left__rot( ptr );
        // GParent_ptr shall still be valid grandparent;
      }
      // ptr is now outer-node.
      // uncle_ptr is now broken.
      ptr -> _parent -> _color = BLACK;
      GParent_ptr -> _color    = RED;
      right_rot( GParent_ptr );
    } /* end of (ptr's parent is a left child) */
    else if ( ptr->_parent == GParent_ptr->_child_R){
      // case II, parent of ptr is a right child.
      uncle_ptr = GParent_ptr->_child_L;
      // check uncle color.
      if( uncle_ptr != nullptr ){
        if( uncle_ptr -> _color == RED ){
          GParent_ptr->_color  = RED;
          uncle_ptr->_color    = BLACK;
          ptr->_parent->_color = BLACK;
          ptr = GParent_ptr;
          continue;
        }
      }
      // uncle is (nil == nullptr) or BLACK
      if( ptr == ptr->_parent->_child_L ){
        // inner-node.
        ptr = ptr->_parent;
        right_rot( ptr );
      }
      // ptr is now outer-node.
      // uncle_ptr is now broken.
      ptr -> _parent -> _color = BLACK;
      GParent_ptr -> _color    = RED;
      left__rot( GParent_ptr );
    }else{
      assert( 0 && "wtf, parent is not child of grandparent" );
    }
  }
  _root -> _color = BLACK;
}

template<typename T>
void
BSTree<T>::delete_fix( BSTreeNode<T>* ptr,
    BSTreeNode<T>* my_parent,
    bool is_left_ch){
  while( (ptr == nullptr || ptr -> _color == BLACK ) &&
      (ptr != _root )) {
    if( is_left_ch ){
      // left child.
      if( my_parent -> _child_R != nullptr &&
          my_parent -> _child_R -> _color == RED ){
        my_parent -> _color = RED;
        my_parent -> _child_R -> _color = BLACK;
        left__rot( my_parent);
      }
      if( my_parent -> _child_R != nullptr ){
        if( my_parent->_child_R->_child_L == nullptr ||
            my_parent->_child_R->_child_L -> _color == BLACK ){
          if( my_parent->_child_R->_child_R == nullptr ||
              my_parent->_child_R->_child_R -> _color == BLACK ){
            // too many black in both side.
            my_parent -> _child_R -> _color = RED;
            ptr = my_parent;
            my_parent = ptr->_parent;
            // valid operation
            // since ptr is never root before this two line.
          }
        }
      }

    }else{
      // right child.
    }
  }
}

#endif // BST_H
