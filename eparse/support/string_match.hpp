#pragma once

#include <iterator>
#include <type_traits>
#include <algorithm>

namespace ep { namespace support {

template < typename I , typename I2 , typename F >
bool string_match( I& begin , I const& end ,
    I2 b , I2 e , size_t s ,
    F&& func ,
    std::enable_if_t< 
      std::is_same< std::random_access_iterator_tag , typename std::iterator_traits<I>::iterator_category >::value
    >* = nullptr )
{
  if( end-begin >= s && std::equal( begin , begin+s , b , e , func ) )
  {
    begin += s;
    return true;
  }
  return false;
}
template < typename I , typename I2 >
bool string_match( I& begin , I const& end ,
    I2 b , I2 e , size_t s ,
    std::enable_if_t<
      std::is_same< std::random_access_iterator_tag , typename std::iterator_traits<I>::iterator_category >::value
    >* = nullptr )
{
  if( end-begin >= s && std::equal( begin , begin+s , b , e ) )
  {
    begin += s;
    return true;
  }
  return false;
}
template < typename I , typename I2 , typename F >
bool string_match( I& begin , I const& end ,
    I2 b , I2 e , size_t s ,
    F&& func ,
    std::enable_if_t<
      std::is_same< std::random_access_iterator_tag , typename std::iterator_traits<I>::iterator_category >::value==false
    >* = nullptr )
{
  while( b != e )
  {
    if( begin != end && func( *begin++ , *b++ ) )
    {
      continue;
    }
    return false;
  }
  return true;
}
template < typename I , typename I2 >
bool string_match( I& begin , I const& end ,
    I2 b , I2 e , size_t s ,
    std::enable_if_t<
      std::is_same< std::random_access_iterator_tag , typename std::iterator_traits<I>::iterator_category >::value==false
    >* = nullptr )
{
  while( b != e )
  {
    if( begin != end && (*begin++ == *b++ ) )
    {
      continue;
    }
    return false;
  }
  return true;
}

}}
