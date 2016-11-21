#pragma once

#include "../core/unused_type.hpp"

#include <vector>
#include <iterator>

namespace ep { namespace support {

template < typename T >
struct vector_shell
{
  using type = std::vector< T >;

  static void push_back( type& vector , T const& obj )
  {
    vector.push_back( obj );
  }
  static void push_back( type& vector , T&& obj )
  {
    vector.push_back( static_cast< T&& >( obj ) );
  }
};
template < typename T >
struct vector_shell< std::vector<T> >
{
  using type = std::vector< T >;

  static void push_back( type& vector , type const& obj )
  {
    vector.insert( vector.end() , obj.begin() , obj.end() );
  }
  static void push_back( type& vector , type&& obj )
  {
    vector.insert( vector.end() , 
        std::make_move_iterator( obj.begin() ) , 
        std::make_move_iterator( obj.end() )
    );
  }
};
template <>
struct vector_shell< core::unused_type >
{
  using type = core::unused_type;

  static void push_back( core::unused_type vector , core::unused_type obj )
  {
  }
};

}}
