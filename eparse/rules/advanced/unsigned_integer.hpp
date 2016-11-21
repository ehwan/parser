#pragma once
#include "unsigned_integer_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../traits/attribute_of_fwd.hpp"
#include "../primitive/range.hpp"
#include "../define.hpp"

#include <type_traits>
#include <cstdint>
#include <utility>

namespace ep { namespace rules { namespace advanced {

template < unsigned int Base , typename T , typename Parser >
class UnsignedInteger
  : public core::expression< UnsignedInteger<Base,T,Parser> >
{
protected:
  Parser parser_;

public:
  constexpr UnsignedInteger( Parser parser )
    : parser_( std::move(parser) )
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
        ret = ret*Base + *i;
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

template < unsigned int Base=10 , typename T = unsigned int , typename DigitParser >
constexpr rules::advanced::UnsignedInteger< Base , T , std::decay_t<DigitParser> >
unsigned_integer( DigitParser&& digitparser )
{
  return { static_cast< DigitParser&& >( digitparser ) };
}
constexpr auto uint = unsigned_integer( digit );

template < unsigned int Base , typename T=unsigned int >
constexpr auto uint_ = unsigned_integer<Base,T>( digit_<Base> );

}

namespace ep { namespace traits {

template < unsigned int Base , typename T , typename P , typename I >
struct attribute_of< rules::advanced::UnsignedInteger<Base,T,P> , I >
{
  using type = T;
};

}}
