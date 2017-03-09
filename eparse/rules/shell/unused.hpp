#pragma once

#include "unused_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/unused_type.hpp"
#include "../../core/attribute_of_fwd.hpp"

#include <utility>

namespace ep { namespace rules { namespace shell {

template < typename P >
class Unused
  : public core::expression< Unused<P> >
{
protected:
  P parser_;
public:
  constexpr Unused( P parser )
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
  core::optional_t< core::unused_type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    if( parser().parse( begin , end , skipper ) )
    {
      return core::unused_type();
    }
    return core::none;
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    return parser().parse( begin , end , skipper );
  }
};

}}}

namespace ep {

template < typename P >
constexpr rules::shell::Unused<P>
unused( P parser )
{
  return { std::move(parser) };
}

}

namespace ep { namespace core {

template < typename P , typename I >
struct attribute_of< rules::shell::Unused<P> , I >
{
  using type = core::unused_type;
};

}}
