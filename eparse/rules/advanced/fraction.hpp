#pragma once

#include "fraction_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../traits/attribute_of_fwd.hpp"
#include "../primitive/range.hpp"

#include <type_traits>
#include <utility>

namespace ep { namespace rules { namespace advanced {

template < unsigned int Base , typename T , typename Parser >
class Fraction
  : public core::expression< Fraction<Base,T,Parser> >
{
protected:
  Parser parser_;

public:
  constexpr Fraction( Parser parser )
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
    constexpr T inv = T( 1.0/Base );
    if( auto i = parser().parse_attribute( begin , end , skipper ) )
    {
      T exp = inv;

      T ret = exp * (*i);
      while( (i = parser().parse_attribute( begin , end , skipper )) )
      {
        exp *= inv;
        ret += exp * (*i);
      }
      return ret;
    }
    begin = begin_;
    return core::none;
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
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

template < unsigned int Base=10 , typename T = float , typename DigitParser >
constexpr rules::advanced::Fraction< Base , T , std::decay_t<DigitParser> >
fraction( DigitParser&& digitparser )
{
  return { static_cast< DigitParser&& >( digitparser ) };
}
constexpr auto float_fraction = fraction<10,float>( digit );
constexpr auto double_fraction = fraction<10,double>( digit );
constexpr auto ldouble_fraction = fraction<10,long double>( digit );

}

namespace ep { namespace traits {

template < unsigned int Base , typename T , typename Parser , typename I >
struct attribute_of< rules::advanced::Fraction<Base,T,Parser> , I >
{
  using type = T;
};

}}
