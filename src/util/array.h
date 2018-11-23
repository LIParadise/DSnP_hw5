/****************************************************************************
  FileName     [ array.h ]
  PackageName  [ util ]
  Synopsis     [ Define dynamic array package ]
  Author       [ Chung-Yang (Ric) Huang ]
  Copyright    [ Copyleft(c) 2005-present LaDs(III), GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ARRAY_H
#define ARRAY_H

#include <cassert>
#include <algorithm>

using namespace std;

// NO need to implement class ArrayNode
//
template <class T>
class Array
{
public:
   // TODO: decide the initial value for _isSorted
   Array() : _data(0), _size(0), _capacity(0) {}
   ~Array() { delete []_data; }

   // DO NOT add any more data member or function for class iterator
   class iterator
   {
      friend class Array;

   public:
      iterator(T* n= 0): _node(n) {}
      iterator(const iterator& i): _node(i._node) {}
      ~iterator() {} // Should NOT delete _node

      // TODO ... done 1118 afternoon: implement these overloaded operators
      const T& operator * () const ;
      T& operator * () ;     
      iterator& operator ++ () ;
      iterator operator ++ (int) ;
      iterator& operator -- () ;
      iterator operator -- (int) ;

      iterator operator + (int ) const ;
      iterator& operator += (int ) ;

      iterator& operator = (const iterator& ) ;

      bool operator != (const iterator& ) const ;
      bool operator == (const iterator& ) const ;

   private:
      T*    _node;
   };

   // TODO ... done 1118 1746: implement these functions
   iterator begin() const ;
   iterator end() const ;
   bool empty() const ;
   size_t size() const ;

   T& operator [] (size_t ) ;
   const T& operator [] (size_t ) const ;

   void push_back(const T& ) ;
   void pop_front() ;
   void pop_back() ;

   bool erase(iterator ) ;
   bool erase(const T& ) ;

   iterator find(const T& ) ;

   void clear() ;

   // [Optional TODO] Feel free to change, but DO NOT change ::sort()
   void sort() const { if (!empty()) ::sort(_data, _data+_size); }

   // Nice to have, but not required in this homework...
   // void reserve(size_t n) { ... }
   // void resize(size_t n) { ... }

private:
   // [NOTE] DO NOT ADD or REMOVE any data member
   T*            _data;
   size_t        _size;       // number of valid elements
   size_t        _capacity;   // max number of elements
   mutable bool  _isSorted;   // (optionally) to indicate the array is sorted

   // [OPTIONAL TODO] Helper functions; called by public member functions
};

template<class T>
const T& Array<T>::iterator::operator * () const {
  return (*_node );
}

template<class T>
T& Array<T>::iterator::operator * () {
  return (*_node );
}

template<class T>
class Array<T>::iterator& Array<T>::iterator::operator ++ () {
  // pre-increment operator
  ++_node;
  return (*this);
}

template<class T>
class Array<T>::iterator& Array<T>::iterator::operator -- () {
  // pre-decrement operator
  --_node;
  return (*this);
}

template<class T>
class Array<T>::iterator Array<T>::iterator::operator ++ (int i) {
  // post-increment operator
  Array<T>::iterator ret = *this;
  _node++;
  return ret;
}

template<class T>
class Array<T>::iterator Array<T>::iterator::operator -- (int i) {
  // post-decrement operator
  Array<T>::iterator ret = *this;
  _node--;
  return ret;
}

template<class T>
class Array<T>::iterator
Array<T>::iterator::operator + (int i ) const{
  return ( Array<T>::iterator( ( _node + i ) ) );
}

template<class T>
class Array<T>::iterator&
Array<T>::iterator::operator += ( int i ){
  _node += i;
  return *this;
}

template<class T>
class Array<T>::iterator&
Array<T>::iterator::operator = (const iterator& other ){
  _node = other._node;
}

template<class T>
bool
Array<T>::iterator::operator != (const iterator& other ) const{
  return ( ! ( this->operator == ( other ) ) );
}

template<class T>
bool
Array<T>::iterator::operator == (const iterator& other ) const{
  return ( _node == other._node );
}

template<class T>
class Array<T>::iterator Array<T>::begin() const {
  return Array<T>::iterator( _data );
}

template<class T>
class Array<T>::iterator Array<T>::end() const {
  return Array<T>::iterator( _data+_size );
}

template<class T>
bool Array<T>::empty() const {
  return ( this->size() == 0 );
}

template<class T>
size_t Array<T>::size() const {
  return _size ;
}

template<class T>
T& Array<T>::operator [] (size_t i) {
  return _data[i];
}

template<class T>
const T& Array<T>::operator [] (size_t i) const {
  return _data[i];
}

template<class T>
void Array<T>::push_back( const T& other){
  if( _size == _capacity ){
    T* tmp_ptr = _data;
    if( _capacity == 0 ){
      _capacity = 1;
    }else{
      _capacity += _capacity;
    }
    _data = new T[ _capacity ];
    for( size_t i = 0; i < _size; ++i )
      _data[i] = tmp_ptr[i];
    delete[] tmp_ptr;
  }
  _data[_size] = other;
  _size++;
#ifdef DEBUG
  assert( _size <= _capacity && "array push_back error" );
#endif // DEBUG
}

template<class T>
void Array<T>::pop_front() {
  if( this->empty() )
    return;
  std::swap( _data[0], _data[_size-1] );
  this->pop_back();
  return;
}

template<class T>
void Array<T>::pop_back() {
  if( this->empty() )
    return;
  _size--;
  (_data+_size) -> ~T();
}

template<class T>
bool Array<T>::erase( Array<T>::iterator it ){
  // document:
  // return false iff i'm empty.
  // no need to check if in range.
  if( this->empty() )
    return false;
  
  std::swap( *(it._node), _data[_size-1] );
  this->pop_back();
  return true;
}

template<class T>
bool Array<T>::erase( const T& other ){
  size_t pos = 0;
  bool found = false;
  for( ; pos < _size; ++pos ){
    if( other == _data[pos] ){
      std::swap( _data[pos], _data[_size-1] );
      this->pop_back();
      found = true;
      break;
    }
  }

  return found;
}

template<class T>
class Array<T>::iterator Array<T>::find ( const T& other){
  for( size_t i = 0; i < _size; ++i ){
    if( other == _data[i] ){
      return iterator( _data + i );
    }
  }
  return this->end() ;
}

template<class T>
void Array<T>::clear() {
  for( size_t i = 0; i < _size; ++i )
    (_data+i)->~T();
  _size = 0;
}

#endif // ARRAY_H
