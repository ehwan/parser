#pragma once

#include "bad_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../traits/attribute_of_fwd.hpp"

#include <exception>
#include <utility>

namespace ep { namespace rules { namespace shell {

template < typename P >
class Bad
  : public core::expression< Bad<P> >
{
protected:
  P parser_;
public:
  constexpr Bad( P parser )
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

  template < typename I >
  void throw_exception( I& begin , I const& end ) const
  {
    throw std::runtime_error( "parse failed" );
  }

  template < typename I , typename S >
  core::optional_t< typename traits::attribute_of<P,I>::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    if( auto i = parser().parse_attribute( begin , end , skipper ) )
    {
      return i;
    }
    throw_exception( begin , end );
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    if( parser().parse( begin , end , skipper ) )
    {
      return true;
    }
    throw_exception( begin , end );
  }

  template < typename ... Ts >
  constexpr auto operator ()( Ts&& ... args ) const;
};

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Bad< std::decay_t<P> >
bad( P&& parser )
{
  return { static_cast< P&& >( parser ) };
}

}

namespace ep { namespace rules { namespace shell {

template < typename P >
template < typename ... Ts >
constexpr auto 
Bad<P>::operator ()( Ts&& ... args ) const
{
  return bad( parser_( static_cast<Ts&&>( args )... ) );
}

}}}

namespace ep { namespace traits {

template < typename P , typename I >
struct attribute_of< rules::shell::Bad<P> , I >
{
  using type = typename attribute_of<P,I>::type;
};

}}
