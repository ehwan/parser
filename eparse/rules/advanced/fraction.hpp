#pragma once

#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../traits/attribute_of_fwd.hpp"
#include "../primitive/range.hpp"

#include <type_traits>
#include <utility>

namespace ep { namespace rules { namespace advanced {

template < typename T , typename Parser >
class Fraction
  : public core::expression< Fraction<T,Parser> >
{
protected:
  Parser parser_;
  T inv_base_;

public:
  constexpr Fraction( Parser parser , T inv_base )
    : parser_( std::move(parser) ) ,
      inv_base_( inv_base )
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
  T invBase() const
  {
    return inv_base_;
  }

  template < typename I , typename S >
  core::optional_t< T >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    if( auto i = parser().parse_attribute( begin , end , skipper ) )
    {
      T exp = inv_base_;

      T ret = exp * (*i);
      while( (i = parser().parse_attribute( begin , end , skipper )) )
      {
        exp *= inv_base_;
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

template < typename T = float , typename DigitParser >
constexpr rules::advanced::Fraction< T , DigitParser >
fraction( DigitParser digitparser , unsigned int base=10 )
{
  return { std::move(digitparser) , T(1)/base };
}
constexpr auto float_fraction = fraction<float>( digit );
constexpr auto double_fraction = fraction<double>( digit );
constexpr auto ldouble_fraction = fraction<long double>( digit );

}

namespace ep { namespace traits {

template < typename T , typename Parser , typename I >
struct attribute_of< rules::advanced::Fraction<T,Parser> , I >
{
  using type = T;
};

}}
