#pragma once

#include "result_of_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/attribute_of_fwd.hpp"

#include <utility>

namespace ep { namespace rules { namespace shell {

template < typename P >
class ResultOf
  : public core::expression< ResultOf<P> >
{
protected:
  P parser_;

public:
  constexpr ResultOf( P parser )
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
  core::optional_t<bool>
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    return core::optional_t<bool>{ parser().parse( begin , end , skipper ) };
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
constexpr rules::shell::ResultOf<P>
result_of( P p )
{
  return { std::move(p) };
}

}

namespace ep { namespace core {

template < typename P , typename I >
struct attribute_of< rules::shell::ResultOf<P> , I >
{
  using type = bool;
};

}}
