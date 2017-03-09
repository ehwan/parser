#pragma once

#include "ref_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/attribute_of_fwd.hpp"

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename P >
class Ref
  : public core::expression< Ref<P> >
{
  using parser_type = std::remove_const_t<P>;
protected:
  P* parser_;

public:
  Ref( P* parser )
    : parser_( parser )
  {
  }

  P& parser()
  {
    return *parser_;
  }
  std::add_const_t<P>& parser() const
  {
    return *parser_;
  }

  P* operator->()
  {
    return parser_;
  }
  std::add_const_t<P>* operator->() const
  {
    return parser_;
  }

  template < typename I , typename S >
  core::optional_t< typename core::attribute_of<parser_type,I>::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    return parser().parse_attribute( begin , end , skipper );
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    return parser().parse( begin , end , skipper );
  }

};

}}}

namespace ep { namespace core {

template < typename Crtp >
rules::shell::Ref<Crtp> expression<Crtp>::ref() &
{
  return { static_cast<Crtp*>(this) };
}
template < typename Crtp >
rules::shell::Ref<Crtp const> expression<Crtp>::ref() const&
{
  return { static_cast<Crtp const*>(this) };
}

}}

namespace ep {

template < typename P>
rules::shell::Ref<P> ref( core::expression<P>& p )
{
  return p.ref();
}
template < typename P>
rules::shell::Ref<P const> ref( core::expression<P> const& p )
{
  return p.ref();
}

}

namespace ep { namespace core {

template < typename P , typename I >
struct attribute_of< rules::shell::Ref<P> , I >
{
  using type = typename attribute_of<
    std::remove_const_t<P> , I
  >::type;
};

}}
