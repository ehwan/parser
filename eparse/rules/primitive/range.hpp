#pragma once

#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/nothing.hpp"
#include "../../core/attribute_of_fwd.hpp"

namespace ep { namespace rules { namespace primitive {

class Range
  : public core::expression< Range >
{
public:
  using value_type = unsigned int;
protected:
  value_type begin_ , end_;
  
public:
  constexpr Range( value_type begin , value_type end )
    : begin_( begin ) ,
      end_( end )
  {
  }

  bool check( unsigned int x ) const
  {
    return begin_ <= x && x <= end_;
  }

  template < typename I >
  core::optional_t<value_type>
  parse_attribute( I& begin , I const& end , core::nothing_t ) const
  {
    if( begin != end )
    {
      const value_type i = *begin;
      if( check(i) )
      {
        ++begin;
        return value_type( i - begin_ );
      }
    }
    return core::none;
  }
  template < typename I >
  bool parse( I& begin , I const& end , core::nothing_t ) const
  {
    if( begin != end )
    {
      if( check(*begin) )
      {
        ++begin;
        return true;
      }
    }
    return false;
  }

  constexpr value_type begin() const
  {
    return begin_;
  }
  constexpr value_type end() const
  {
    return end_;
  }
};

}}}

namespace ep {

constexpr rules::primitive::Range
range( unsigned int begin , unsigned int end )
{
  return { begin , end };
}
constexpr auto lower = range( 'a' , 'z' );
constexpr auto upper = range( 'A' , 'Z' );
constexpr auto digit = range( '0' , '9' );
constexpr auto whitespace = range(
  10 , 13
);
constexpr auto print = range(
  32 , 126
);
constexpr auto graph = range(
  33 , 126
);

}

namespace ep { namespace core {
  template < typename I >
  struct attribute_of< rules::primitive::Range , I >
  {
    using type = rules::primitive::Range::value_type;
  };
}}
