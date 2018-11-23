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
#include <algorithm>

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
   bool erase(iterator ) ;
   bool erase(const T& ) ;

   iterator find(const T& ) ;

   void clear()  ; // delete all nodes except for the dummy node

   void sort() const ;

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   DListNode<T>*  _head;     // = dummy node if list is empty
   mutable bool   _isSorted; // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] helper functions; called by public member functions
   void           my_iterator_swap_content( iterator&, iterator& ) const;
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
  return DList<T>::iterator ( _head->_prev );
}

template <class T>
bool
DList<T>::empty() const {
  return begin()._node == end()._node;
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
  DListNode<T>* ptr = new DListNode<T>( other, (--end())._node,
      end()._node);
  DList<T>::iterator it = end();
  // it points to dummy node.
  bool first_ele = empty();

  (--end())._node->_next = ptr;
  end()    ._node->_prev = ptr;
  if( first_ele )
    _head = ptr;
}

template <class T>
void 
DList<T>::pop_front() {
  if( empty() )
    return;

  auto ptr = (++begin())._node;

  // list is non-empty;
#ifdef DEBUG
  assert( erase( begin() ) && "pop_front error in dlist" );
#else
  erase( begin() );
#endif

  _head = ptr;

}

template <class T>
void
DList<T>::pop_back() {
  if( empty() )
    return;

#ifdef DEBUG
  assert( erase( --end() ) && "pop_back error in dlist" );
#else
  erase( --end() );
#endif

}

template <class T>
bool
DList<T>::erase( DList<T>::iterator pos ){
  if( empty() || pos == end() )
    return false;

  bool one_ele = false;
  if( (++begin()) == end() )
    one_ele = true;

  auto next_it = pos;
  auto prev_it = pos;
  ++next_it; // next element of pos;
  --prev_it; // prev element of pos;
  next_it . _node -> _prev = prev_it . _node;
  prev_it . _node -> _next = next_it . _node;
  (*pos).~T();
  pos = next_it;

  if( one_ele )
    _head = end()._node;

  return true;
}

template <class T>
bool
DList<T>::erase( const T& other ){
  if( empty() )
    return false;

  // we need _head != dummy iff !empty()
  // s.t. we would not delete dummy node.
  auto it = find ( other );
  if( it == end () ){
    return false;
  }else {
    erase( it );
    return true;
  }
}

template <class T>
class DList<T>::iterator
DList<T>::find( const T& other ){
  auto it = begin();
  for( ; it != end(); ++it ){
    if( *it == other )
      return it;
  }
  return end();
}

template <class T>
void
DList<T>::clear() {
  while( !empty() )
    pop_front();
}

template <class T>
void
DList<T>::sort() const {
  if( empty() )
    return;
  // we'll use bubble sort.

  const auto start_it  = begin();
  auto end_it    = end();
  auto it2 = start_it;
  auto it1 = it2++;

  for( ; start_it != end_it;
      --end_it , it2 = start_it, it1 = it2++){
    // it1 = start_it, it2 = (it1 + 1);
    for( ; it2 != end_it; ++it1, ++it2 ){
      // for( ; (ry); it1+=1, it2+=1 )
      if( *it1 > *it2 )
        my_iterator_swap_content ( it1, it2 );
    }
  }
}

template <class T>
void
DList<T>::my_iterator_swap_content ( DList<T>::iterator& it1,
    DList<T>::iterator& it2 ) const {
  std::swap( (it1._node->_data), (it2._node->_data) );
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

template <class T>
class DList<T>::iterator&
DList<T>::iterator::operator ++ () {
  // pre-increment;
  _node = _node -> _next;
  return *this;
}

template <class T>
class DList<T>::iterator&
DList<T>::iterator::operator -- () {
  // pre-decrement;
  _node = _node -> _prev;
  return *this;
}

template <class T>
class DList<T>::iterator
DList<T>::iterator::operator ++ (int dummy ) {
  // post-increment;
  auto it = *this;
  this->operator ++();
  return it;
}

template <class T>
class DList<T>::iterator
DList<T>::iterator::operator -- (int dummy ) {
  // post-increment;
  auto it = *this;
  this->operator --();
  return it;
}

template <class T>
class DList<T>::iterator&
DList<T>::iterator::operator = ( const iterator& i ) {
  _node = i._node;
  return (*this);
}

template <class T>
bool
DList<T>::iterator::operator != ( const iterator& i ) const {
  return ( ! ( i == (*this) ) );
}

template <class T>
bool
DList<T>::iterator::operator == ( const iterator& i ) const {
  return i._node == this->_node; 
}
#endif // DLIST_H
