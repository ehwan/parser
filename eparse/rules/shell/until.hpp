#pragma once

#include "until_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../traits/attribute_of_fwd.hpp"

#include <utility>

namespace ep { namespace rules { namespace shell {

template < typename P >
class Until
  : public core::expression< Until<P> >
{
protected:
  P parser_;

public:
  constexpr Until( P parser )
    : parser_( std::move(parser) )
  {
  }

  P& parser()
  {
    return parser_;
  }
  P const& parser() const
  {
    return parser_;
  }

  template < typename I , typename S >
  core::optional_t< typename traits::attribute_of<P,I>::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    while( 1 )
    {
      if( auto i = parser().parse_attribute( begin , end , skipper ) ){ return *i; }
    }
    return core::none;
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    while( 1 )
    {
      if( parser().parse( begin , end , skipper ) ){ return true; }
    }
    return false;
  }

  template < typename ... Ts >
  constexpr auto operator ()( Ts&& ... args ) const;
};

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Until< std::decay_t<P> >
until( P&& parser )
{
  return { static_cast< P&& >( parser ) };
}

}

namespace ep { namespace rules { namespace shell {

template < typename P >
template < typename ... Ts >
constexpr auto 
Until<P>::operator ()( Ts&& ... args ) const
{
  return until( parser_( static_cast<Ts&&>( args )... ) );
}

}}}

namespace ep { namespace traits {

template < typename P , typename I >
struct attribute_of< rules::shell::Until<P> , I >
{
  using type = typename attribute_of<P,I>::type;
};

}}
