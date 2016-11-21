#pragma once

#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/nothing.hpp"
#include "../../traits/attribute_of_fwd.hpp"

namespace ep { namespace rules { namespace primitive {

template < typename T >
class Range
  : public core::expression< Range<T> >
{
protected:
  T begin_ , end_;
  
public:
  constexpr Range( T begin , T end )
    : begin_( begin ) ,
      end_( end )
  {
  }

  template < typename I >
  core::optional_t<T>
  parse_attribute( I& begin , I const& end , core::nothing_t ) const
  {
    if( begin != end )
    {
      const T i = *begin;
      if( i >= begin_ && i < end_ )
      {
        ++begin;
        return T( i - begin_ );
      }
    }
    return core::none;
  }
  template < typename I >
  bool parse( I& begin , I const& end , core::nothing_t ) const
  {
    if( begin != end )
    {
      const T i = *begin;
      if( i >= begin_ && i <= end_ )
      {
        ++begin;
        return true;
      }
    }
    return false;
  }

  constexpr T begin() const
  {
    return begin_;
  }
  constexpr T end() const
  {
    return end_;
  }

  constexpr Range<T> operator ()( T size ) const
  {
    return { begin() , begin() + size - 1 };
  }
};

}}}

namespace ep {

template < typename T = unsigned int >
constexpr rules::primitive::Range<T>
range( T begin , T end )
{
  return { begin , end };
}
constexpr auto lower = range<unsigned int>( 'a' , 'z' );
constexpr auto upper = range<unsigned int>( 'A' , 'Z' );
constexpr auto digit = range<unsigned int>( '0' , '9' );
constexpr auto bdigit = digit(2);
constexpr auto odigit = digit(8);
constexpr auto whitespace = range<unsigned int>(
  10 , 13
);
constexpr auto print = range<unsigned int>(
  32 , 126
);
constexpr auto graph = range<unsigned int>(
  33 , 126
);

}

namespace ep { namespace traits {
  template < typename T , typename I >
  struct attribute_of< rules::primitive::Range<T> , I >
  {
    using type = T;
  };
}}
