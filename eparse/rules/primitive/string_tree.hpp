#pragma once

#include "../../core/expression.hpp"
#include "../../core/unused_type.hpp"
#include "../../core/optional.hpp"
#include "../../traits/attribute_of_fwd.hpp"
#include "../../support/string_tree_node.hpp"

#include <functional>

namespace ep { namespace rules { namespace primitive {

template < typename Crtp , typename CharT , typename T >
class StringTreeBase
  : public core::expression<Crtp>
{
  using node_type = support::PackedStringNode<CharT,T>;
protected:
  node_type root_;

  auto& _isequal()
  {
    return static_cast<Crtp*>(this)->isequal();
  }
  auto& _isequal() const
  {
    return static_cast<Crtp const*>(this)->isequal();
  }

public:
  
  void insert( CharT const* str , T val , unsigned int strsize=0 )
  {
    root_.get_back( str , strsize==0?std::char_traits<CharT>::length(str):strsize , _isequal() ).emplace( std::move(val) );
  }
  typename node_type::data_type& value( CharT const* str , unsigned int strsize=0 )
  {
    return root_.get_back( str , strsize==0?std::char_traits<CharT>::length(str):strsize , _isequal() );
  }
  typename node_type::data_type& operator []( CharT const* str )
  {
    return value( str );
  }
  void print() const
  {
    root_.print( -1 );
  }

  void shrink_to_fit()
  {
    root_.shrink_to_fit();
  }
  
  template < typename I , typename S >
  core::optional_t< T >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    node_type const* node = &root_;
    T const* saved = nullptr;
    I savedit = begin_;
    while(1)
    {
      if( begin != end )
      {
        if( auto findres = node->find( *begin , _isequal() ) )
        {
          node = &(**findres);
          if( node->check( begin , end , _isequal() ) )
          {
            if( node->data_ )
            {
              saved = &(*(node->data_));
              savedit = begin;
            }
            continue;
          }
        }
        if( savedit != begin_ )
        {
          begin = savedit;
          return { *saved };
        }
      }
      begin = begin_;
      return core::none;
    }
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    node_type const* node = &root_;
    I savedit = begin_;
    while(1)
    {
      if( begin != end )
      {
        if( auto findres = node->find( *begin , _isequal() ) )
        {
          node = &(**findres);
          if( node->check( begin , end , _isequal() ) )
          {
            if( node->data_ )
            {
              savedit = begin;
            }
            continue;
          }
        }
        if( savedit != begin_ )
        {
          begin = savedit;
          return true;
        }
      }
      begin = begin_;
      return false;
    }
  }
};
template < typename CharT , typename T , typename Equal >
class StringTree
  : public StringTreeBase< StringTree<CharT,T,Equal> , CharT , T >
{
protected:
  Equal isequal_;

public:
  StringTree( Equal isequal )
    : isequal_( std::move(isequal) )
  {
  }
  StringTree()
  {
  }
  Equal& isequal()
  {
    return isequal_;
  }
  Equal const& isequal() const
  {
    return isequal_;
  }
};
template < typename CharT , typename T >
class StringTree< CharT , T , std::function< bool(CharT,CharT) > >
  : public StringTreeBase< StringTree<CharT,T,std::function<bool(CharT,CharT)>> , CharT , T >
{
  using function_type = std::function< bool(CharT,CharT) >;
protected:
  function_type isequal_;

public:
  StringTree( function_type isequal )
    : isequal_( std::move(isequal) )
  {
  }
  StringTree()
    : isequal_( std::equal_to<CharT>() )
  {
  }
  function_type& isequal()
  {
    return isequal_;
  }
  function_type const& isequal() const
  {
    return isequal_;
  }
  function_type& isequal( function_type rhs )
  {
    isequal_ = std::move(rhs);
    return isequal_;
  }
};

}}}

namespace ep {

template < typename T , typename CharT = char , typename Filter=std::equal_to<CharT> >
using string_tree = rules::primitive::StringTree< CharT , T , Filter >;

}

namespace ep { namespace traits {

template < typename C , typename T , typename F , typename I >
struct attribute_of< rules::primitive::StringTree<C,T,F> , I >
{
  using type = T;
};

}}
