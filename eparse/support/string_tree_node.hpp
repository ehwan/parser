#pragma once

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include "../core/optional.hpp"
#include "string_match.hpp"

namespace ep { namespace support {

template < typename CharT , typename CharTraits , typename T >
struct StringTreeNode
{
  struct compare_t
  {
    bool operator()( CharT a , CharT b ) const
    {
      return CharTraits::lt( a , b );
    }
  };
  struct equals_t
  {
    bool operator()( CharT a , CharT b ) const
    {
      return CharTraits::eq( a , b );
    }
  };
  using this_type = StringTreeNode<CharT,CharTraits,T>;
  using string_type = std::basic_string<CharT,CharTraits>;
  using data_type = core::optional_t<T>;
  using map_type = std::map<CharT,this_type,compare_t>;


  string_type string_;
  data_type data_;
  map_type children_;

  StringTreeNode()
  {
  }
  StringTreeNode( string_type string , data_type data , map_type children )
    : string_( std::move(string) ) ,
      data_( std::move(data) ) ,
      children_( std::move(children) )
  {
  }

  this_type const* find( CharT ch ) const
  {
    auto res = children_.find( ch );
    if( res != children_.end() )
    {
      return &res->second;
    }
    return nullptr;
  }
  this_type* find( CharT ch )
  {
    auto res = children_.find( ch );
    if( res != children_.end() )
    {
      return &res->second;
    }
    return nullptr;
  }
  template < typename I >
  bool check( I& begin , I const& end ) const
  {
    ++begin;
    if( string_.size() == 1 ){ return true; }
    return support::string_match( begin , end , string_.begin()+1 , string_.end() , string_.size()-1 ,
        equals_t() );
  }

  void separate( unsigned int end )
  {
    if( end >= string_.size() ){ return; }
    map_type new_children = std::move(children_);
    children_.clear();
    children_.emplace( 
      string_[end] ,
      this_type( string_.substr(end) , std::move(data_) , std::move(new_children) )
    );
    string_.resize( end );
    string_.shrink_to_fit();
  }

  data_type& get_front( CharT const* str , unsigned int strsize )
  {
    //front must match
    //if( strsize == 0 ){ return; }

    auto i = str;
    auto si = string_.begin();
    while(1)
    {
      //                i
      // string : input/*****
      //          input
      if( i == str + strsize )
      {
        separate( i - str );
        return data_;
      }

      // string : ***
      //          inp/ut
      //              i
      if( si == string_.end() )
      {
        return get_back( i , static_cast<unsigned int>( str + strsize - i ) );
      }

      //             i
      // string : **/******
      //          in/put
      if( CharTraits::eq( *i , *si ) == false )
      {
        const unsigned int dist = i - str;
        separate( dist );
        return children_.emplace( 
          *i ,
          this_type( string_type(i,strsize-dist) , core::none , map_type() )
        ).first->second.data_;
      }

      ++i;
      ++si;
    }
  }
  data_type& get_back( CharT const* str , unsigned int len )
  {
    if( auto findres = find( *str ) )
    {
      return findres->get_front( str , len );
    }else
    {
      return children_.emplace( 
        *str ,
        this_type( string_type(str,len) , core::none , map_type() )
      ).first->second.data_;
    }
  }

  void print( int level=0 ) const
  {
    for( int i=0; i<level; ++i )
    {
      std::cout << ' ';
    }
    if( level >= 0 )
    {
      std::cout << string_ << " : " << (data_?"O":"X") << std::endl;
    }
    for( auto& i : children_ )
    {
      i.second.print( level + 1 );
    }
  }
};

}}
