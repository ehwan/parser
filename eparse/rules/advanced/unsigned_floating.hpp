#pragma once

#include "unsigned_floating_fwd.hpp"
#include "fraction.hpp"
#include "unsigned_integer.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../traits/attribute_of_fwd.hpp"

#include "../primitive/one.hpp"
#include "../primitive/range.hpp"
#include <utility>

namespace ep { namespace rules { namespace advanced {

template < unsigned int Base , typename T , typename Parser >
class UnsignedFloating
  : public core::expression< UnsignedFloating<Base,T,Parser> >
{
protected:
  Parser parser_;
  bool need_dot_;

public:
  constexpr UnsignedFloating( Parser parser , bool need_dot )
    : parser_( std::move(parser) ) ,
      need_dot_( need_dot )
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
  bool needDot() const
  {
    return need_dot_;
  }

  template < typename I , typename S >
  core::optional_t< T >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    if( auto integer = unsigned_integer<Base,T>( parser() ).parse_attribute( begin , end , skipper ) )
    {
      I begin_beforedot = begin;
      if( one('.').parse( begin , end , skipper ) )
      {
        if( auto fract = fraction<Base,T>( parser() ).parse_attribute( begin , end , skipper ) )
        {
          return *integer + *fract;
        }
      }
      if( need_dot_ )
      {
        begin = begin_;
        return core::none;
      }else
      {
        begin = begin_beforedot;
        return *integer;
      }
    }
    begin = begin_;
    return core::none;
  }
  template < typename I , typename S >
  bool parser( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    if( unsigned_integer<Base,T>( parser() ).parse( begin , end , skipper ) )
    {
      I begin_beforedot = begin;
      if( one('.').parse( begin , end , skipper ) && 
          fraction<Base,T>( parser() ).parse( begin , end , skipper ) )
      {
        return true;
      }else
      {
        if( need_dot_ )
        {
          begin = begin_;
          return false;
        }
        begin = begin_beforedot;
        return true;
      }
    }
    begin = begin_;
    return false;
  }
};

}}}

namespace ep {

template < unsigned int Base=10 , typename T = float , typename DigitParser >
constexpr rules::advanced::UnsignedFloating< Base , T , std::decay_t<DigitParser> >
unsigned_floating( DigitParser&& digitparser , bool need_dot = true )
{
  return { static_cast< DigitParser&& >( digitparser ) , need_dot };
}
template < typename T = float >
constexpr auto
unsigned_floating()
{
  return unsigned_floating<10,T>( digit );
}
constexpr auto ufloat = unsigned_floating<10,float>( digit );
constexpr auto udouble = unsigned_floating<10,double>( digit );
constexpr auto uldouble = unsigned_floating<10,long double>( digit );

}

namespace ep { namespace traits {

template < unsigned int Base , typename T , typename P , typename I >
struct attribute_of< rules::advanced::UnsignedFloating<Base,T,P> , I >
{
  using type = T;
};

}}
