#pragma once

#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/attribute_of_fwd.hpp"
#include "../primitive/range.hpp"
#include "../define.hpp"

#include <type_traits>
#include <cstdint>
#include <utility>

namespace ep { namespace rules { namespace advanced {

template < typename T , typename Parser >
class UnsignedInteger
  : public core::expression< UnsignedInteger<T,Parser> >
{
protected:
  Parser parser_;
  unsigned int base_;

public:
  constexpr UnsignedInteger( Parser parser , unsigned int base )
    : parser_( std::move(parser) ) ,
      base_( base )
  {
  }

  Parser& parser()
  {
    return parser_;
  }
  Parser const& parser() const
  {
    return parser_;
  }
  unsigned int base() const
  {
    return base_;
  }

  template < typename I , typename S >
  core::optional_t< T >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    if( auto i = parser().parse_attribute( begin , end , skipper ) )
    {
      T ret = static_cast<T>( *i );
      while( (i = parser().parse_attribute( begin , end , skipper )) )
      {
        ret = ret*base_ + *i;
      }
      return ret;
    }
    begin = begin_;
    return core::none;
  }
  template < typename I , typename S >
  bool parser( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    if( parser().parse( begin , end , skipper ) )
    {
      while( parser().parse( begin , end , skipper ) )
      {
      }
      return true;
    }
    begin = begin_;
    return false;
  }
};

}}}

namespace ep {

template < typename T = unsigned int , typename DigitParser >
constexpr rules::advanced::UnsignedInteger< T , std::decay_t<DigitParser> >
unsigned_integer( DigitParser&& digitparser , unsigned int base=10 )
{
  return { static_cast< DigitParser&& >( digitparser ) , base };
}
constexpr auto uint = unsigned_integer( digit );
constexpr auto uint16 = unsigned_integer( xdigit , 16 );

}

namespace ep { namespace core {

template < typename T , typename P , typename I >
struct attribute_of< rules::advanced::UnsignedInteger<T,P> , I >
{
  using type = T;
};

}}
