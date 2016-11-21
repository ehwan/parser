#pragma once

#include "../core/unused_type.hpp"

namespace ep { namespace support {

template < typename Begin >
struct range_begin_helper_t
{
  template < typename T >
  constexpr static bool apply( Begin begin , T x )
  {
    return begin <= x;
  }
  template < typename T >
  constexpr static T offset( Begin begin , T x )
  {
    return x - begin;
  }
};
template <>
struct range_begin_helper_t< core::unused_type >
{
  template < typename T >
  constexpr static bool apply( core::unused_type , T x )
  {
    return true;
  }
  template < typename T >
  constexpr static T offset( core::unused_type , T x )
  {
    return 0;
  }
};
template < typename End >
struct range_end_helper_t
{
  template < typename T >
  constexpr static bool apply( T x , End end )
  {
    return x < end;
  }
  template < typename T >
  constexpr static bool end( T x , End end )
  {
    return x == end;
  }
};
template <>
struct range_end_helper_t< core::unused_type >
{
  template < typename T >
  constexpr static bool apply( T x , core::unused_type )
  {
    return true;
  }
  template < typename T >
  constexpr static bool end( T x , core::unused_type )
  {
    return false;
  }
};
template < typename Begin , typename End >
struct range
{
  Begin begin_;
  End end_;

  constexpr range( Begin begin , End end )
    : begin_( begin ) ,
      end_( end )
  {
  }
  constexpr range()
    : begin_() ,
      end_()
  {
  }
  constexpr Begin begin() const
  {
    return begin_;
  }
  constexpr End end() const
  {
    return end_;
  }

  template < typename T >
  constexpr bool operator ()( T x ) const
  {
    return range_begin_helper_t<Begin>::apply( begin() , x ) &&
           range_end_helper_t<End>::apply( x , end() );
  }
  template < typename T >
  constexpr bool end( T x ) const
  {
    return range_end_helper_t<End>::end( x , end() );
  }
  template < typename T >
  constexpr T offset( T x ) const
  {
    return range_begin_helper_t<Begin>::offset( begin() , x );
  }
};

template < typename T >
struct range_one
{
  T data_;

  constexpr range_one( T data )
    : data_( data )
  {
  }
  constexpr range_one()
    : data_()
  {
  }
  constexpr T data() const
  {
    return data_;
  }

  constexpr bool operator ()( T i ) const
  {
    return i == data_;
  }
  constexpr bool end( T ) const
  {
    return true;
  }
  constexpr T offset( T ) const
  {
    return 0;
  }
};

}}
