#pragma once

#include "../core/optional.hpp"

namespace ep { namespace support {

template < typename T , typename I >
core::optional_t<T>
parse_little_endian( I& begin , I const& end , unsigned int c )
{
  I begin_ = begin;
  T ret{ 0 };
  unsigned int passed = 0u;
  while( c-- )
  {
    if( begin != end )
    {
      ret += static_cast<T>( *(begin++) ) << (passed*8);
      ++passed;
    }else
    {
      begin = begin_;
      return core::none;
    }
  }
  return ret;
}
template < typename T , typename I >
core::optional_t<T>
parse_big_endian( I& begin , I const& end , unsigned int c )
{
  I begin_ = begin;
  T ret{ 0 };
  while( c-- )
  {
    if( begin != end )
    {
      ret = (ret<<8) + static_cast<T>( *(begin++) );
    }else 
    {
      begin = begin_;
      return core::none;
    }
  }
  return ret;
}

}}
