/****************************************************************************
  FileName     [ dlist.h ]
  PackageName  [ util ]
  Synopsis     [ Define doubly linked list package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef DLIST_H
#define DLIST_H

#include <cassert>

template <class T> class DList;

// DListNode is supposed to be a private class. User don't need to see it.
// Only DList and DList::iterator can access it.
//
// DO NOT add any public data member or function to this class!!
//
template <class T>
class DListNode
{
   friend class DList<T>;
   friend class DList<T>::iterator;

   DListNode(const T& d, DListNode<T>* p = 0, DListNode<T>* n = 0):
      _data(d), _prev(p), _next(n) {}

   // [NOTE] DO NOT ADD or REMOVE any data member
   T              _data;
   DListNode<T>*  _prev;
   DListNode<T>*  _next;
};


template <class T>
class DList
{
public:
   // TODO: decide the initial value for _isSorted
   DList() {
      _head = new DListNode<T>(T());
      _head->_prev = _head->_next = _head; // _head is a dummy node
   }
   ~DList() { clear(); delete _head; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class DList;

   public:
      iterator(DListNode<T>* n= 0): _node(n) {}
      iterator(const iterator& i) : _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO: implement these overloaded operators
      // we'll make end() equal to dummy, always.
      // begin() == end() == dummy iff empty.
      const T& operator * () const ;
      T& operator * () ;
      iterator& operator ++ () ;
      iterator operator ++ (int) ;
      iterator& operator -- () ;
      iterator operator -- (int) ;

      iterator& operator = (const iterator& i) ;

      bool operator != (const iterator& ) const ;
      bool operator == (const iterator& ) const ;

   private:
      DListNode<T>* _node;
   };

   // TODO: implement these functions
   iterator begin() const ;
   iterator end() const ;
   bool empty() const ;
   size_t size() const ;

   void push_back(const T& ) ;
   void pop_front() ;
   void pop_back() ;

   // return false if nothing to erase
   bool erase(iterator pos) ;
   bool erase(const T& ) ;

   iterator find(const T& ) ;

   void clear()  ; // delete all nodes except for the dummy node

   void sort() const ;

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
};

template <class T>
class DList<T>::iterator
DList<T>::begin() const {
  // return end() if empty.
  return DList<T>::iterator( _head );
}

template <class T>
class DList<T>::iterator
DList<T>::end() const {
  DList<T>::iterator it ( _head );
  return (--it);
}

template <class T>
bool
DList<T>::empty() const {
  return ( ( _head -> _next) == _head );
}

template <class T>
size_t
DList<T>::size() const {
  size_t ret = 0;
  auto it = begin();
  auto end_it = end();
  while( it != end_it ){
    ++it;
    ++ret;
  }
  return ret;
}

template <class T>
void
DList<T>::push_back( const T& other) {
  DListNode<T>* ptr = new DListNode<T>(other);
  DList<T>::iterator it = end();
  // it points to dummy node.
  bool first_ele = empty();

  (--end())._node->_next = ptr;
  ptr -> _prev = (--end())._node;
  end()    ._node->_prev = ptr;
  ptr -> _next = it._node;
  if( first_ele )
    _head = ptr;
}

template <class T>
void 
DList<T>::pop_front() {
  if( empty() )
    return;

  bool one_ele = false;
  if( (++begin()) == end() )
    one_ele = true;

  // list is non-empty;
  auto tmp_it = begin();
  end()._node->_next = tmp_it._node->_next;
  tmp_it._node->_next->_prev = end()._node;
  tmp_it._node -> _data.~T();
  if( one_ele )
    _head = end()._node;
}

template <class T>
T&
DList<T>::iterator::operator * () {
  return _node->_data;
}

template <class T>
const T&
DList<T>::iterator::operator * () const {
  return _node->_data;
}

#endif // DLIST_H
