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

    char color;
};


template <class T>
class BSTree
{
  public:
   // TODO: design your own class!!
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

  void insert( const AdtTestObj& );
  void print();
};

#endif // BST_H
