#pragma once

#include "fraction.hpp"
#include "unsigned_integer.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/attribute_of_fwd.hpp"

#include "../primitive/one.hpp"
#include "../primitive/range.hpp"
#include <utility>

namespace ep { namespace rules { namespace advanced {

template < typename T , typename Parser , typename DotParser >
class UnsignedFloating
  : public core::expression< UnsignedFloating<T,Parser,DotParser> >
{
protected:
  Parser parser_;
  DotParser dotparser_;
  bool need_dot_;
  unsigned int base_;

public:
  constexpr UnsignedFloating( Parser parser , DotParser dparser , bool need_dot , unsigned int base )
    : parser_( std::move(parser) ) ,
      dotparser_( std::move(dparser) ) ,
      need_dot_( need_dot ) ,
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
  DotParser& dotParser()
  {
    return dotparser_;
  }
  DotParser const& dotParser() const
  {
    return dotparser_;
  }
  bool needDot() const
  {
    return need_dot_;
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
    if( auto integer = unsigned_integer<T>( parser() , base_ ).parse_attribute( begin , end , skipper ) )
    {
      I begin_beforedot = begin;
      if( dotparser_.parse( begin , end , skipper ) )
      {
        if( auto fract = fraction<T>( parser() , base_ ).parse_attribute( begin , end , skipper ) )
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
    if( unsigned_integer<T>( parser() , base_ ).parse( begin , end , skipper ) )
    {
      I begin_beforedot = begin;

      if( dotparser_.parse( begin , end , skipper ) && 
          fraction<T>( parser() , base_ ).parse( begin , end , skipper ) )
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

template < typename T = float , typename DigitParser , typename DotParser >
constexpr rules::advanced::UnsignedFloating< T , DigitParser , DotParser >
unsigned_floating( DigitParser digitparser , DotParser dotparser , bool need_dot = true , unsigned int base=10 )
{
  return { std::move(digitparser) , std::move(dotparser) , need_dot , base };
}
template < typename T = float >
constexpr auto
unsigned_floating( bool need_dot=true )
{
  return unsigned_floating<T>( digit , one('.') , need_dot );
}
constexpr auto ufloat = unsigned_floating<float>();
constexpr auto udouble = unsigned_floating<double>();
constexpr auto uldouble = unsigned_floating<long double>();

}

namespace ep { namespace core {

template < typename T , typename P , typename D , typename I >
struct attribute_of< rules::advanced::UnsignedFloating<T,P,D> , I >
{
  using type = T;
};

}}
