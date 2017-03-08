#pragma once

#include "../../core/expression.hpp"
#include "../../core/unused_type.hpp"
#include "../../core/optional.hpp"
#include "../../traits/attribute_of_fwd.hpp"
#include "../../support/string_tree_node.hpp"

#include <functional>

namespace ep { namespace rules { namespace primitive {

template < typename CharT , typename CharTraits , typename T >
class StringTree
  : public core::expression< StringTree<CharT,CharTraits,T> >
{
  using node_type = support::StringTreeNode<CharT,CharTraits,T>;

protected:
  node_type root_;

public:
  void insert( CharT const* str , unsigned int len , T value )
  {
    root_.get_back( str , len ).emplace( std::move(value) );
  }
  void insert( CharT const* str , T value )
  {
    root_.get_back( str , CharTraits::length(str) ).emplace( std::move(value) );
  }
  typename node_type::data_type&
  operator []( CharT const* str )
  {
    return root_.get_back( str , CharTraits::length(str) );
  }
  void print() const
  {
    root_.print( -1 );
  }
  template < typename I , typename S >
  core::optional_t<T>
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    node_type const* current_node = &root_;
    T const* last_succeed = nullptr;
    I last_succeed_it = begin_;
    while(1)
    {
      if( begin != end )
      {
        if( auto findres = current_node->find( *begin ) )
        {
          if( findres->check( begin , end ) )
          {
            current_node = findres;
            if( findres->data_ )
            {
              last_succeed = &(*findres->data_);
              last_succeed_it = begin;
            }
            continue;
          }
        }
      }
      goto failphase;
    }

failphase:
    if( last_succeed )
    {
      begin = last_succeed_it;
      return *last_succeed;
    }else
    {
      begin = begin_;
      return core::none;
    }
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    node_type const* current_node = &root_;
    I last_succeed_it = begin_;
    while(1)
    {
      if( begin != end )
      {
        if( auto findres = current_node->find( *begin ) )
        {
          if( findres->check( begin , end ) )
          {
            current_node = findres;
            if( findres->data )
            {
              last_succeed_it = begin;
            }
            continue;
          }
        }
      }
      goto failphase;
    }

failphase:
    if( last_succeed_it != begin_ )
    {
      begin = last_succeed_it;
      return true;
    }else
    {
      begin = begin_;
      return false;
    }
  }
};

}}}

namespace ep {

template < typename T , typename CharT=char , typename CharTraits=std::char_traits<CharT> >
using string_tree = rules::primitive::StringTree< CharT , CharTraits , T >;

}

namespace ep { namespace traits {

template < typename C , typename CT , typename T , typename I >
struct attribute_of< rules::primitive::StringTree<C,CT,T> , I >
{
  using type = T;
};

}}
