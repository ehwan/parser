#pragma once

#include "capture_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../traits/attribute_of_fwd.hpp"

#include <utility>

namespace ep { namespace rules { namespace shell {

template < typename P >
class Capture
{
protected:
  P parser_;
public:
  constexpr Capture( P parser )
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
  core::optional_t< std::pair<I,I> >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    if( parser().parse( begin , end , skipper ) )
    {
      return std::pair<I,I>( begin_ , begin );
    }
    return core::none;
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    return parser().parse( begin , end , skipper );
  }

  template < typename ... Ts >
  constexpr auto operator ()( Ts&& ... args ) const;
};

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Capture< std::decay_t<P> >
capture( P&& parser )
{
  return { static_cast< P&& >( parser ) };
}

}

namespace ep { namespace rules { namespace shell {

template < typename P >
template < typename ... Ts >
constexpr auto 
Capture<P>::operator ()( Ts&& ... args ) const
{
  return capture( parser_( static_cast<Ts&&>( args )... ) );
}

}}}

namespace ep { namespace traits {

template < typename P , typename I >
struct attribute_of< rules::shell::Capture<P> , I >
{
  using type = std::pair< I , I >;
};

}}
