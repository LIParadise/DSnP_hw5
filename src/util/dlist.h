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
   void           my_merge_sort( 
       const iterator&, const iterator& ) const;
   iterator       my_find_mid_itor( 
       const iterator&, const iterator& ) const;
   void           my_merge( const iterator&,
       const iterator&, const iterator& ) const;
};

template <class T>
class DList<T>::iterator
DList<T>::begin() const {
  // return end() if empty.
  return _head->_next;
}

template <class T>
class DList<T>::iterator
DList<T>::end() const {
  return const_cast<DListNode<T>*>(_head);
}

template <class T>
bool
DList<T>::empty() const {
  return begin() == end();
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

  (--end())._node->_next = ptr;
  end()    ._node->_prev = ptr;
}

template <class T>
void 
DList<T>::pop_front() {
  if( empty() )
    return;

  // list is non-empty;
#ifdef DEBUG
  assert( erase( begin() ) && "pop_front error in dlist" );
#else
  erase( begin() );
#endif
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

  auto next_it = pos;
  auto prev_it = pos;
  ++next_it; // next element of pos;
  --prev_it; // prev element of pos;
  next_it . _node -> _prev = prev_it . _node;
  prev_it . _node -> _next = next_it . _node;
  delete pos._node;
  pos = next_it;

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
  if( it == end() ){
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
  // we'll implement merge_sort.

  my_merge_sort( begin(), end() );
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
DList<T>::iterator::operator != ( 
    const DList<T>::iterator& i ) const {
  return ( ! ( i == (*this) ) );
}

template <class T>
bool
DList<T>::iterator::operator == ( 
    const DList<T>::iterator& i ) const {
  return i._node == _node; 
}

template <class T>
void
DList<T>::my_merge_sort( const DList<T>::iterator& start_it,
    const DList<T>::iterator& end_it ) const {

  DList<T>::iterator my_start_it = start_it ;

  if( my_start_it == end_it || ++my_start_it == end_it )
    return;

#ifdef DEBUG
  my_start_it = start_it;
  while( my_start_it != end_it ){
    ++ my_start_it;
    if( my_start_it == end() && end_it != end() )
      assert( 0 && "mergesort error" );
  }
#endif // DEBUG

  DList<T>::iterator my_mid_it =
      my_find_mid_itor( start_it, end_it );

  my_merge_sort( start_it, my_mid_it );
  my_merge_sort( my_mid_it, end_it );

  my_merge( start_it, my_mid_it, end_it );

}

template <typename T>
typename DList<T>::iterator
DList<T>::my_find_mid_itor ( const DList<T>::iterator& start_it,
    const DList<T>::iterator& end_it ) const {

  auto it1 = start_it;
  auto it2 = end_it;

  while( it1 != it2 && ++it1 != it2 )
    --it2;

  return it1;
}

template <typename T>
void
DList<T>::my_merge( const DList<T>::iterator& start_it,
    const DList<T>::iterator& my_mid_it,
    const DList<T>::iterator& end_it ) const { 

  auto it1 = start_it;
  auto it2 = my_mid_it;
  assert( it1 != end() && it2 != end() && "merge failure");

  while( it2 != end_it && it1 != my_mid_it){
    if( *it1 > *it2 ){
      // O o o o o X x x x x _
      // ^         ^         ^
      // it1       it2       end_it
      // we want to modify the list s.t. it becomes
      // X O o o o o x x x x _
      // ^ ^         ^       ^
      // T it1       it2     end_it
      // where T means old_it2
      auto old_it2 = it2++;

      // oXx --> ox
      old_it2._node->_prev->_next = old_it2._node->_next;
      old_it2._node->_next->_prev = old_it2._node->_prev;

      // Ooo... --> XOoo...
      // notice we have a ring.
      old_it2._node->_prev        = it1._node->_prev;
      old_it2._node->_next        = it1._node;
      it1._node->_prev->_next     = old_it2._node;
      it1._node->_prev            = old_it2._node;

    }else{
      // O o o o o X x x x x _
      // ^         ^         ^
      // it1       it2       end_it
      // we want to modify the list s.t. it becomes
      // O o o o o X x x x x _
      //   ^       ^         ^
      //   it1     it2       end_it
      it1++;
    }
  }

}

#endif // DLIST_H
