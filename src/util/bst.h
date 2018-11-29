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
      data(t), parent(ptrP), child_R(ptrR), child_L(ptrL), color(0){}
    T              data;
    char           color;
    BSTreeNode<T>* parent;
    BSTreeNode<T>* child_R;
    BSTreeNode<T>* child_L;
    // 0 for red.
    // 1 for black.
    // 2 for red-and-black, used in deletion.
    // 3 for doubly-black, used in deletion.
};


template <class T>
class BSTree
{
  public:
    // TODO: design your own class!!
    BSTree(): root( nullptr ) {}
    class iterator {
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
    };
    void    pop_front();
    void    pop_back();
    void    sort();
    size_t  size();
    bool    empty();
    bool    erase( const AdtTestObj& );
    // just erase first encountered.
    // return false iff not found.
    bool    erase( iterator pos );
    // pos would be valid except when ADT.empty() in this homework.
    // pos invalid iff ADT is empty, return false in this case.
    iterator find();
    iterator end();
    iterator begin();

    void insert( const T& );
    void print();
  private:
    BSTreeNode<T>* root;
};

template<typename T>
void
BSTree<T>::insert( const T& other ){
  if( empty() )
    root = new BSTreeNode( other );
  else{
    bool done = false;
    auto* ptr = root;
    while( !done ){
      if( other > ptr->data ){
        if( ptr->child_R == nullptr ){
          ptr->child_R = new BSTreeNode( other, ptr);
          done = true;
        }else{
          ptr = ptr->child_R;
          continue;
        }
      }else{
        if( ptr->child_L == nullptr ){
          ptr->child_L = new BSTreeNode( other, ptr);
          done = true;
        }else{
          ptr = ptr->child_L;
          continue;
        }
      }
    }
  }
}

#endif // BST_H
