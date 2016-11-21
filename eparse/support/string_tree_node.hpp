#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "../core/optional.hpp"
#include "string_match.hpp"

namespace ep { namespace support {

template < typename CharT , typename T >
struct PackedStringNode
{
  using this_type = PackedStringNode<CharT,T>;
  using map_type = std::vector<this_type>;
  using data_type = core::optional_t<T>;
  using string_type = std::basic_string<CharT>;

  string_type string_;
  data_type data_;
  map_type child_;

  PackedStringNode()
  {
  }
  PackedStringNode( string_type string , data_type data , map_type child )
    : string_( std::move(string) ) ,
      data_( std::move(data) ) ,
      child_( std::move(child) )
  {
  }

  template < typename I , typename Equal >
  bool check( I& begin , I const& end , Equal& isequal ) const
  {
    ++begin;
    return support::string_match( begin , end , string_.begin()+1 , string_.end() , string_.size()-1 , isequal );
  }

  template < typename Equal >
  core::optional_t<typename map_type::const_iterator>
  find( CharT ch , Equal& isequal ) const
  {
    for( auto i=child_.begin(); i!=child_.end(); ++i )
    {
      if( isequal( i->string_.front() , ch ) )
      {
        return i;
      }
    }
    return core::none;
  }
  template < typename Equal >
  core::optional_t<typename map_type::iterator>
  find( CharT ch , Equal& isequal )
  {
    for( auto i=child_.begin(); i!=child_.end(); ++i )
    {
      if( isequal( i->string_.front() , ch ) )
      {
        return i;
      }
    }
    return core::none;
  }

  void separate( unsigned int end )
  {
    if( end >= string_.size() ){ return; }
    map_type new_child = std::move( child_ );
    child_.clear();
    child_.emplace_back( string_.substr(end) , std::move(data_) , std::move(new_child) );
    string_.resize( end );
    string_.shrink_to_fit();
  }

  template < typename Equal >
  data_type& get_front( CharT const* str , unsigned int strsize , Equal& isequal )
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
        return get_back( i , static_cast<unsigned int>( str + strsize - i ) ,
            isequal );
      }

      //             i
      // string : **/******
      //          in/put
      if( isequal( *i , *si ) == false )
      {
        const unsigned int dist = i - str;
        separate( dist );
        return get_back( i , strsize - dist ,
            isequal );
      }

      ++i;
      ++si;
    }
  }
  template < typename Equal >
  data_type& get_back( CharT const* str , unsigned int strsize , Equal& isequal )
  {
    //if( strsize == 0 ){ return; }
    if( auto res = find( *str , isequal ) )
    {
      return (*res)->get_front( str , strsize , isequal );
    }else
    {
      child_.emplace_back( string_type( str , strsize ) , core::none , map_type() );
      return child_.back().data_;
    }
  }

  void shrink_to_fit()
  {
    child_.shrink_to_fit();
    string_.shrink_to_fit();
    for( auto& i : child_ )
    {
      i.shrink_to_fit();
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
    for( auto& i : child_ )
    {
      i.print( level + 1 );
    }
  }
};

}}
