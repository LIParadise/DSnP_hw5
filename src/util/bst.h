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
    class iterator {
      public:
        friend class BSTree;
        iterator():_ptr(nullptr) {}
        iterator(BSTreeNode<T>* p, const BSTree<T>* hp = nullptr):
          _ptr(p), _hidden_parent( hp ) {}
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
        BSTreeNode<T>* _ptr;
        const BSTree<T>*    _hidden_parent ;
    };
    friend class iterator;
    BSTree(): _root( nullptr ), _size(0) {}
    void    pop_front();
    void    pop_back();
    void    clear();
    void    clear( BSTreeNode<T>* );
    void    sort() const;
    size_t  size() const;
    bool    empty() const;
    bool    erase( BSTreeNode<T>* );
    bool    erase( const T& );
    // just erase first encountered.
    // return false iff not found.
    bool    erase( iterator pos );
    // pos would be valid except when ADT.empty() in this homework.
    // pos invalid iff ADT is empty, return false in this case.
    static BSTreeNode<T>* successor  ( const iterator&  ) ;
    static BSTreeNode<T>* successor  ( BSTreeNode<T>*   ) ;
    static BSTreeNode<T>* predecessor( const iterator&  ) ;
    static BSTreeNode<T>* predecessor( BSTreeNode<T>*   ) ;
    iterator find(const T&);
    BSTreeNode<T>* find(const T&, BSTreeNode<T>* );
    iterator       end()   const;
    iterator       begin() const;

    void insert( const T& );
    void print() const ;
    BSTreeNode<T>* max () const;
    BSTreeNode<T>* min () const;
  private:
    static const int RED   = 0;
    static const int BLACK = 1;
    BSTreeNode<T>* _root;
    void insert_fix( BSTreeNode<T>*& );
    void delete_fix( BSTreeNode<T>*, BSTreeNode<T>*, char);
    // false mean _child_R;
    // true  mean _child_L;
    void right_rot ( BSTreeNode<T>* );
    void left__rot ( BSTreeNode<T>* );
    void check_sort( BSTreeNode<T>* ) const;
    void transplant( BSTreeNode<T>*, BSTreeNode<T>* );
    void print     ( BSTreeNode<T>*, size_t, char = 0 ) const;
    BSTreeNode<T>* min_sub ( BSTreeNode<T>* ) const;
    BSTreeNode<T>* max_sub ( BSTreeNode<T>* ) const;
    size_t _size;
};

template<typename T>
const T&
BSTree<T>::iterator::operator * () const {
  return _ptr -> _data;
}

template<typename T>
T&
BSTree<T>::iterator::operator * () {
  return _ptr -> _data;
}

template<typename T>
typename BSTree<T>::iterator&
BSTree<T>::iterator::operator ++ () {
  // pre-increment operator ++;
  _ptr = BSTree<T>::successor( _ptr );
  return *this;
}

template<typename T>
typename BSTree<T>::iterator
BSTree<T>::iterator::operator ++ (int dummy ){
  // post-increment operator ++;
  auto* ret = _ptr;
  this->operator ++();
  return ret;
}

template<typename T>
typename BSTree<T>::iterator&
BSTree<T>::iterator::operator -- () {
  // pre-increment operator --;
  if( _ptr != nullptr )
    _ptr = BSTree<T>::predecessor( _ptr );
  else
    _ptr = _hidden_parent -> max() ;
  return *this;
}

template<typename T>
typename BSTree<T>::iterator
BSTree<T>::iterator::operator -- (int dummy ){
  // post-increment operator --;
  auto* ret = _ptr;
  this->operator --();
  return ret;
}

template<typename T>
typename BSTree<T>::iterator&
BSTree<T>::iterator::operator = (const iterator& i ){
  _ptr = i._ptr;
}

template<typename T>
bool
BSTree<T>::iterator::operator == (const iterator&i ) const {
  return ( _ptr == (i._ptr) );
}

template<typename T>
bool
BSTree<T>::iterator::operator != (const iterator&i ) const {
  return !(this->operator == (i));
}

template<typename T>
void
BSTree<T>::pop_front () {
  if( empty() )
    return;
#ifdef DEBUG
  assert( erase( min() ) && "pop_front error" );
#else
  erase( min() );
#endif // DEBUG
}

template<typename T>
void
BSTree<T>::pop_back() {
  if( empty() )
    return;
#ifdef DEBUG
  assert( erase( max() ) && "pop_back error" );
#else
  erase( max() );
#endif // DEBUG
}

template<typename T>
void 
BSTree<T>::clear() {
  clear( _root );
  _root = nullptr;
  _size = 0;
}

template<typename T>
void
BSTree<T>::clear( BSTreeNode<T>* ptr ){
  if( ptr == nullptr )
    return;
  else {
    if( ptr -> _child_L != nullptr )
      clear( ptr -> _child_L );
    if( ptr -> _child_R != nullptr )
      clear( ptr -> _child_R );
    delete ptr;
  }
}

template<typename T>
void
BSTree<T>::sort() const {
#ifdef DEBUG
  check_sort( _root );
#endif // DEBUG
}

template<typename T>
void 
BSTree<T>::check_sort ( BSTreeNode<T>* ptr ) const{
  if( ptr -> _child_L != nullptr ){
    check_sort( ptr -> _child_L );
    assert(ptr->_child_L->_data <= ptr->_data && "sort err");
  }
  if( ptr -> _child_R != nullptr ){
    check_sort( ptr -> _child_R );
    assert(ptr->_child_R->_data >= ptr->_data && "sort err");
  }
}

template<typename T>
size_t
BSTree<T>::size() const {
  return _size;
}

template<typename T>
bool
BSTree<T>::empty() const {
#ifdef DEBUG
  if( _size == 0 ){
    assert( _root == nullptr && "empty  error" );
    return true;
  }else{
    assert( _root != nullptr && "!empty error" );
    return false;
  }
#else
  return _size == 0;
#endif // DEBUG
}

template<typename T>
bool
BSTree<T>::erase( BSTreeNode<T>* ptr ) {
  if( ptr == nullptr )
    return false;
  if( ptr == _root && _size == 1 ){
    _size = 0;
    delete _root;
    _root = nullptr;
    return true;
  }

  char color_bak = ptr -> _color;
  BSTreeNode<T>* successor_ptr = nullptr;
  _size --;
  char ch = 0;

  if( ptr != _root ){
    if( ptr == ptr -> _parent -> _child_R )
      ch = 'R';
    else if( ptr == ptr -> _parent -> _child_L )
      ch = 'L';
    else
      assert( 0 && "wait what???????? erase(ptr)." );
  }

  if( ptr -> _child_L == nullptr ){
    successor_ptr = ptr -> _child_R;
    transplant( ptr, successor_ptr );
    if( color_bak == BLACK )
      delete_fix( successor_ptr, ptr -> _parent, ch );
  }else if( ptr -> _child_R == nullptr ){
    successor_ptr = ptr -> _child_L;
    transplant( ptr, successor_ptr );
    if( color_bak == BLACK )
      delete_fix( successor_ptr, ptr -> _parent, ch );
  }else{
    successor_ptr = min_sub( ptr -> _child_R );
#ifdef DEBUG
    if( successor_ptr == nullptr ){
      assert( 0 && "wtf in successor(ptr)" );
    }
#endif // DEBUG
    color_bak = successor_ptr -> _color;
    auto* need_fix        = successor_ptr -> _child_R;
    auto* need_fix_parent = successor_ptr;
    ch = 'R';
    if( successor_ptr -> _parent != ptr ){
      ch = 'L';
      need_fix_parent = successor_ptr -> _parent;
      transplant( successor_ptr, successor_ptr -> _child_R );
      successor_ptr -> _child_R = ptr -> _child_R;
      successor_ptr -> _child_R -> _parent  = successor_ptr;
    }
    transplant( ptr, successor_ptr );
    successor_ptr -> _child_L = ptr -> _child_L;
    successor_ptr -> _child_L -> _parent = successor_ptr;
    successor_ptr -> _color = ptr -> _color;
    // safe, since ptr have two children
    // and successor_ptr._child_L  shall be nullptr;
    if( color_bak == BLACK ){
      delete_fix( need_fix, need_fix_parent, ch);
    }
  }
  if( _root == nullptr || _root -> _color == BLACK ){
    delete ptr;
    return true;
  }else if( _root -> _color == RED ){
    delete ptr;
    return false;
  }else {
    assert( 0 && "WTF in erase????" );
  }
}

template<typename T>
bool
BSTree<T>::erase( const T& other){
  return erase ( find( other ) );
}

template<typename T>
bool
BSTree<T>::erase( iterator pos ){
  return ( erase( pos._ptr ) );
}

template<typename T>
BSTreeNode<T>*
BSTree<T>::successor( BSTreeNode<T>* ptr ){
  if( ptr == nullptr )
    return nullptr;
  else if( ptr -> _child_R == nullptr ){
    if( ptr -> _parent == nullptr ){
      return nullptr;
    }
    auto* ptr_bak = ptr;
    do{
      ptr_bak = ptr;
      ptr     = ptr -> _parent;
    }while( ptr != nullptr && ptr_bak != ptr -> _child_L );
  }else {
    ptr = ptr -> _child_R;
    while( ptr -> _child_L != nullptr ){
      ptr = ptr -> _child_L;
    }
  }
  return ptr;
}

template<typename T>
BSTreeNode<T>*
BSTree<T>::predecessor( BSTreeNode<T>* ptr ) {
  if( ptr == nullptr )
    // nil
    return nullptr;
  else if( ptr -> _child_L == nullptr ){
    if( ptr -> _parent == nullptr ){
      return nullptr;
    }
    auto* ptr_bak = ptr;
    do{
      ptr_bak = ptr;
      ptr     = ptr -> _parent;
    }while( ptr != nullptr && ptr_bak != ptr -> _child_R );
  }else {
    ptr = ptr -> _child_L;
    while( ptr -> _child_R != nullptr ){
      ptr = ptr -> _child_R;
    }
  }
  return ptr;
}

template<typename T>
BSTreeNode<T>*
BSTree<T>::predecessor( const iterator& it ){
  return predecessor( it.ptr );
}

template<typename T>
BSTreeNode<T>*
BSTree<T>::successor( const iterator& it ){
  return successor( it.ptr );
}

template<typename T>
typename BSTree<T>::iterator
BSTree<T>::find( const T& other ) {
  return find( other, _root );
}

template<typename T>
BSTreeNode<T>*
BSTree<T>::find( const T& other, BSTreeNode<T>* ptr ){
  if( empty() )
    return nullptr;
  BSTreeNode<T>* tmp_ptr = nullptr;
  if( ptr -> _child_L != nullptr ){
    tmp_ptr = find( other, ptr -> _child_L );
    if( tmp_ptr != nullptr )
      return tmp_ptr;
  }
  if( ptr -> _data == other ){
    return ptr;
  }
  if( ptr -> _child_R != nullptr ){
    tmp_ptr = find( other, ptr -> _child_R );
    if( tmp_ptr != nullptr )
      return tmp_ptr;
  }
  return nullptr;
}

template<typename T>
BSTreeNode<T>*
BSTree<T>::max() const {
  if( empty() )
    return nullptr;
  auto* ptr = _root;
  while( ptr -> _child_R != nullptr )
    ptr = ptr -> _child_R;
  return ptr;
}

template<typename T>
BSTreeNode<T>*
BSTree<T>::min() const {
  if( empty() )
    return nullptr;
  auto* ptr = _root;
  while( ptr -> _child_L != nullptr )
    ptr = ptr -> _child_L;
  return ptr;
}

template<typename T>
BSTreeNode<T>*
BSTree<T>::min_sub( BSTreeNode<T>* ptr ) const {
  if( ptr == nullptr )
    return nullptr;
  else{
    while( ptr -> _child_L != nullptr )
      ptr = ptr -> _child_L;
    return ptr;
  }
}

template<typename T>
BSTreeNode<T>*
BSTree<T>::max_sub( BSTreeNode<T>* ptr ) const {
  if( ptr == nullptr )
    return nullptr;
  else{
    while( ptr -> _child_R != nullptr )
      ptr = ptr -> _child_R;
    return ptr;
  }
}

template<typename T>
typename BSTree<T>::iterator
BSTree<T>::end() const {
  return iterator ( nullptr, this );
}

template<typename T>
typename BSTree<T>::iterator
BSTree<T>::begin() const {
  return iterator( min(), this );
}

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
          ptr = ptr->_child_R;
          done = true;
        }else{
          ptr = ptr->_child_R;
          continue;
        }
      }else{
        if( ptr->_child_L == nullptr ){
          ptr->_child_L = new BSTreeNode<T>( other, ptr);
          ptr = ptr -> _child_L;
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
BSTree<T>::insert_fix( BSTreeNode<T>*& ptr ){
  // ptr must have RED;
  if( ptr == _root ){
    assert( 0 && "WTF, insert_fix met root??" );
  }else if( ptr -> _parent == _root ){
    return;
  }

  // ptr must have grandparent.
  while( ptr != _root && ptr->_parent->_color == RED ){
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
      if( GParent_ptr == _root )
        _root = ptr -> _parent;
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
      if( GParent_ptr == _root )
        _root = ptr -> _parent;
    }else{
      assert( 0 && "wtf, parent is not child of grandparent" );
    }
  }
  _root -> _color = BLACK;
}

template<typename T>
void
BSTree<T>::delete_fix( BSTreeNode<T>* ptr,
    BSTreeNode<T>* my_parent, char which ){
  while( ptr != _root &&  
      ( ptr == nullptr || ptr -> _color == BLACK ) ){
    // we shall always have sibling.
    // we shall have parent when entering the loop.
    if( which == 'L' ){
#ifdef DEBUG
      assert( my_parent != nullptr && "wait wut..., delete_fix" );
      assert( my_parent -> _child_R != nullptr && "wait wut..., delete_fix" );
#endif // DEBUG
      // left child.
      if( my_parent -> _child_R -> _color == RED ){
        // case I.
        my_parent -> _color = RED;
        my_parent -> _child_R -> _color = BLACK;
        left__rot( my_parent);
      }
      if( ( my_parent->_child_R->_child_L == nullptr ||
            my_parent->_child_R->_child_L -> _color == BLACK ) &&
          ( my_parent->_child_R->_child_R == nullptr ||
            my_parent->_child_R->_child_R -> _color == BLACK )) {
        // case II.
        // too many black in both side.
        my_parent -> _child_R -> _color = RED;
        ptr = my_parent;
        my_parent = my_parent->_parent;
        // my_parent may be broken now.
        if( my_parent != nullptr ){
          if( ptr == my_parent -> _child_R )
            which = 'R';
          else if( ptr == my_parent -> _child_L )
            which = 'L';
          else
            assert( 0 && "WTF in case II of rb-delete-fix" );
        }
        continue;
      }
      if( my_parent != nullptr ){
        if( my_parent -> _child_R != nullptr ) {
          if( (my_parent -> _child_R -> _child_R == nullptr ||
                my_parent -> _child_R -> _child_R -> _color == BLACK ) ){

#ifdef DEBUG
            assert( my_parent -> _child_R -> _child_L != nullptr  &&
                "my both nephew are BLACK... WTF dude?!" );
#endif // DEBUG
            my_parent->_child_R->_child_L->_color = BLACK;
            my_parent -> _child_R -> _color = RED;
            right_rot( my_parent -> _child_R );
            // shall be valid
            // since we have ( my_parent -> _child_R -> _child_L being RED. )
          }
#ifdef DEBUG
          assert( my_parent -> _child_R -> _child_R != nullptr &&
              my_parent -> _child_R -> _child_R -> _color == RED &&
              "my opposite nephew have color BLACK in case IV, weird." );
#endif
          my_parent -> _child_R -> _color = my_parent -> _color;
          my_parent -> _child_R -> _child_R -> _color = BLACK;
          my_parent -> _color = BLACK;
          left__rot( my_parent );
          ptr = _root;
          my_parent = nullptr;
        }else {
          // dummy;
#ifdef DEBUG
          assert( 0 && "case III and case IV have NIL sibling?!" );
#endif // debug
        }
      } else {
        // my_parent is nullptr in case III and case IV;
#ifdef DEBUG
        assert( 0 && "case III and case IV have NIL parent?!" );
        cerr << (( _root == ptr )? "I'm root" : "I'm not root....") << endl;
#endif // DEBUG
      }
    }else if( which == 'R' ){
      // right child
#ifdef DEBUG
      assert( my_parent != nullptr && "wait wut..., delete_fix" );
      assert( my_parent -> _child_L != nullptr && "wait wut..., delete_fix" );
#endif // DEBUG
      if( my_parent -> _child_L -> _color == RED ){
        // case I.
        my_parent -> _child_L -> _color = BLACK;
        my_parent -> _color = RED;
        right_rot( my_parent);
      }
      if( ( my_parent->_child_L->_child_L == nullptr ||
            my_parent->_child_L->_child_L -> _color == BLACK ) &&
          ( my_parent->_child_L->_child_R == nullptr ||
            my_parent->_child_L->_child_R -> _color == BLACK )) {
        // case II.
        // too many black in both side.
        my_parent -> _child_L -> _color = RED;
        ptr = my_parent;
        my_parent = my_parent->_parent;
        // my_parent may be broken now.
        if( my_parent != nullptr ){
          if( ptr == my_parent -> _child_R )
            which = 'R';
          else if( ptr == my_parent -> _child_L )
            which = 'L';
          else
            assert( 0 && "WTF in case II of rb-delete-fix" );
        }
        continue;
      }
      if( my_parent != nullptr ){
        if( my_parent -> _child_L != nullptr ) {
          if( (my_parent -> _child_L -> _child_L == nullptr ||
                my_parent -> _child_L -> _child_L -> _color == BLACK ) ){

#ifdef DEBUG
            assert( my_parent -> _child_L -> _child_R != nullptr  &&
                "my both nephew are BLACK... WTF dude?!" );
#endif // DEBUG
            my_parent->_child_L->_child_R->_color = BLACK;
            my_parent -> _child_L -> _color = RED;
            left__rot( my_parent -> _child_L );
            // shall be valid
            // since we have ( my_parent -> _child_L -> _child_R being RED. )
          }
#ifdef DEBUG
          assert( my_parent -> _child_L -> _child_L != nullptr &&
              my_parent -> _child_L -> _child_L -> _color == RED &&
              "my opposite nephew have color BLACK in case IV, weird." );
#endif
          my_parent -> _child_L -> _color = my_parent -> _color;
          my_parent -> _child_L -> _child_L -> _color = BLACK;
          my_parent -> _color = BLACK;
          right_rot( my_parent );
          ptr = _root;
          my_parent = nullptr;
        }else {
          // dummy;
#ifdef DEBUG
          assert( 0 && "case III and case IV have NIL sibling?!" );
#endif // debug
        }
      } else {
        // my_parent is nullptr in case III and case IV;
#ifdef DEBUG
        assert( 0 && "case III and case IV have NIL parent?!" );
        cerr << (( _root == ptr )? "I'm root" : "I'm not root....") << endl;
#endif // DEBUG
      }
    }else {
#ifdef DEBUG
      assert( 0 && "color wrong in delete-fix" );
#endif // DEBUG
    }
  }
  if( ptr != nullptr )
    ptr -> _color = BLACK;
}

template<typename T>
void
BSTree<T>::right_rot( BSTreeNode<T>* ptr ){
#ifdef DEBUG
  assert( ptr->_child_L != nullptr && "right rot error" );
#endif // DEBUG
  /*  suppose we right_rot( A );
  /// assume A._child_L != nullptr.
  /// =======before======
  ///        A
  ///       / \
  ///      B   C
  ///     / \
  ///    D   E                   
  /// =======after=======
  ///        B
  ///       / \
  ///      D   A
  ///         / \
  ///        E   C                
  ///
   */
  auto* B_ptr = ptr->_child_L;
  B_ptr -> _parent = ptr -> _parent;

  if( ptr != _root ){
    if( ptr == ptr->_parent->_child_L )
      ptr->_parent->_child_L = B_ptr;
    else if( ptr == ptr->_parent -> _child_R )
      ptr->_parent->_child_R = B_ptr;
    else
      assert( 0 && "right rot bizzare error" );
  }else{
    _root = B_ptr;
  }

  // set parent of E to A.
  if( B_ptr -> _child_R != nullptr )
    B_ptr -> _child_R -> _parent = ptr;

  // set _child_L of A to E.
  ptr -> _child_L = B_ptr -> _child_R;

  ptr -> _parent = B_ptr;
  B_ptr -> _child_R = ptr;
}

template<typename T>
void
BSTree<T>::left__rot( BSTreeNode<T>* ptr ){
#ifdef DEBUG
  assert( ptr->_child_R != nullptr && "left rot error" );
#endif // DEBUG
  /*  suppose we left__rot( A );
  /// assume A._child_R != nullptr.
  /// =======before======
  ///        A
  ///       / \
  ///      B   C
  ///         / \
  ///        D   E                   
  /// =======after=======
  ///        C
  ///       / \
  ///      A   E
  ///     / \
  ///    B   D                    
  ///
   */
  auto* C_ptr = ptr->_child_R;
  C_ptr -> _parent = ptr -> _parent;

  if( ptr != _root ){
    if( ptr == ptr->_parent->_child_L )
      ptr->_parent->_child_L = C_ptr;
    else if( ptr == ptr->_parent -> _child_R )
      ptr->_parent->_child_R = C_ptr;
    else
      assert( 0 && "left rot bizzare error" );
  }else{
    _root = C_ptr;
  }

  // set parent of D to A.
  if( C_ptr -> _child_L != nullptr )
    C_ptr -> _child_L -> _parent = ptr;

  // set _child_R of A to D.
  ptr -> _child_R = C_ptr -> _child_L;

  ptr -> _parent = C_ptr;
  C_ptr -> _child_L = ptr;
}

template<typename T>
void
BSTree<T>::transplant( BSTreeNode<T>* ptr1, BSTreeNode<T>* ptr2 ){
  // attach ptr2 to ptr1.
  // don't care what ptr1 would be.
#ifdef DEBUG
  assert( ptr1 != nullptr && "wtf in transplant?" );
#endif // DEBUG
  if( ptr1 == _root )
    _root = ptr2;
  else if( ptr1 == ptr1 -> _parent -> _child_R )
    (ptr1 -> _parent -> _child_R) = ptr2;
  else if( ptr1 == ptr1 -> _parent -> _child_L )
    (ptr1 -> _parent -> _child_L) = ptr2;
  else
    assert( 0 && "wtf in transplant????" );
  if( ptr2 != nullptr )
    ptr2 -> _parent = ptr1 -> _parent;
}

template<typename T>
void
BSTree<T>::print() const {
  // print( _root , 0);
  size_t L_black_height = 1;
  size_t R_black_height = 1;
  size_t s              = 0;
  for( auto* ptr = _root; ptr != nullptr; ptr = ptr -> _child_L ){
    if( ptr == nullptr || ptr -> _color == BLACK )
      ++ L_black_height;
  }
  for( auto* ptr = _root; ptr != nullptr; ptr = ptr -> _child_R ){
    if( ptr == nullptr || ptr -> _color == BLACK )
      ++ R_black_height;
  }
  cout << "left  black height: " << L_black_height << endl;
  cout << "right black height: " << R_black_height << endl;
  cout << "stored size is :    " << _size << endl;
  for( auto it = begin(); it != end(); ++it )
    s ++;
  cout << "reach-able size is: " << s << endl;
}

template<typename T>
void 
BSTree<T>::print( BSTreeNode<T>* ptr, size_t indent, char type ) const {
  if( ptr != nullptr ){
    if( ptr != _root ){
      if( ptr != ptr -> _parent -> _child_L && ptr != ptr ->_parent->_child_R )
        cerr << "\033[30;47m" << 
          "something wrong, parent have no this child." <<
          "\033[0m" << endl;
    }
    print( ptr -> _child_L, indent+1, 'L');
    for( size_t i = 0; i < indent; ++i ){
      cout << '\t';
    }
    switch( type ){
      case 'R' :
        cout << "\033[0mRch";
        break;
      case 'L' :
        cout << "\033[0mLch";
        break;
      default:
        cout << "\033[0mRut";
        break;
    }
    if( ptr -> _color == RED )
      cout << "\033[1;31m"  << ptr -> _data << "\033[0m" << endl;
    else if( ptr -> _color == BLACK )
      cout << "\033[30;47m" << ptr -> _data << "\033[0m" << endl;
    else
      assert( 0 && "wtf in printing??" );
    print( ptr -> _child_R, indent+1, 'R' );
  }else{
    for( size_t i = 0; i < indent; ++i ){
      cout << "\033[0m" << '\t';
    }
    switch( type ){
      case 'R' :
        cout << "\033[0mRch";
        break;
      case 'L' :
        cout << "\033[0mLch";
        break;
      default:
        cout << "\033[0mRut";
        break;
    }
    cout << "\033[30;43mNULLPTR\033[0m" << endl;
  }
}

#endif // BST_H
