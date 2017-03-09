#pragma once

#include <string>

namespace ep { namespace support {

template < typename CharT >
struct ci_char_traits
  : public std::char_traits<CharT>
{
  static CharT filter_char( CharT x )
  {
    if( 'A' <= x && x <= 'Z' )
    {
      return x + 32;
    }
    return x;
  }
  static bool eq( CharT a , CharT b )
  {
    return filter_char(a) == filter_char(b);
  }
  static bool lt( CharT a , CharT b )
  {
    return filter_char(a) < filter_char(b);
  }
  static int compare( CharT const* a , CharT const* b , size_t n )
  {
    while( n-- )
    {
      const CharT a_ = filter_char(a);
      const CharT b_ = filter_char(b);

      if( a_ < b_ )
      {
        return -1;
      }
      if( a_ > b_ )
      {
        return 1;
      }
      ++a;
      ++b;
    }
    return 0;
  }
  static CharT const* find( CharT const* s , size_t n , CharT a )
  {
    const CharT a_ = filter_char(a);
    while( n-- )
    {
      if( a_ == filter_char(*s) )
      {
        return s;
      }
      ++s;
    }
    return nullptr;
  }
};

}}

namespace ep {

/// case insensitive char_traits
template < typename CharT >
using ci_char_traits = support::ci_char_traits<CharT>;

}
