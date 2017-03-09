#pragma once

#include "optional_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/attribute_of_fwd.hpp"

#include <utility>

namespace ep { namespace rules { namespace shell {

template < typename P >
class Optional
{
protected:
  P parser_;

public:
  constexpr Optional( P parser )
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
  core::optional_t< core::optional_t< typename core::attribute_of<P,I>::type > >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    using ret_type = core::optional_t< core::optional_t< typename core::attribute_of<P,I>::type > >;
    return ret_type( parser().parse_attribute( begin , end , skipper ) );
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    parser().parse( begin , end , skipper );
    return true;
  }
};

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Optional< std::decay_t<P> >
optional( P&& p )
{
  return { static_cast< P&& >( p ) };
}

}

namespace ep { namespace core {

template < typename P , typename I >
struct attribute_of< rules::shell::Optional<P> , I >
{
  using type = core::optional_t< typename attribute_of<P,I>::type >;
};

}}
