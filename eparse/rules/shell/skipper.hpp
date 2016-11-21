#pragma once

#include "skipper_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/nothing.hpp"
#include "../../traits/attribute_of_fwd.hpp"

#include <utility>


namespace ep { namespace rules { namespace shell {

template < typename P , typename S >
class Skipper
  : public core::expression< Skipper<P,S> >
{
protected:
  P parser_;
  S skipper_;
public:
  constexpr Skipper( P p , S s )
    : parser_( std::move(p) ) ,
      skipper_( std::move(s) )
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
  S& skipper()
  {
    return skipper_;
  }
  S const& skipper() const
  {
    return skipper_;
  }

  template < typename I >
  core::optional_t< typename traits::attribute_of<P,I>::type >
  parse_attribute( I& begin , I const& end , core::nothing_t ) const
  {
    return parser().parse_attribute( begin , end , skipper() );
  }
  template < typename I >
  bool parse( I& begin , I const& end , core::nothing_t ) const
  {
    return parser().parse( begin , end , skipper() );
  }
};

}}}

namespace ep {

template < typename P , typename S >
constexpr rules::shell::Skipper< std::decay_t<P> , std::decay_t<S> >
skipper( P&& p , S&& s )
{
  return { static_cast< P&& >( p ) ,
           static_cast< S&& >( s ) };
}

}

namespace ep { namespace traits {

template < typename P , typename S , typename I >
struct attribute_of< rules::shell::Skipper<P,S> , I >
{
  using type = typename attribute_of<P,I>::type;
};

}}
