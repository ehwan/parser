#pragma once

#include "attribute_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/nothing.hpp"
#include "../../core/attribute_of_fwd.hpp"

#include <utility>

namespace ep { namespace rules { namespace primitive {

template < typename Attr >
class Attribute
  : public core::expression< Attribute<Attr> >
{
public:
  Attr attr_;

  Attribute( Attr attr )
    : attr_( std::move(attr) )
  {
  }

  Attr& attr()
  {
    return attr_;
  }
  Attr const& attr() const
  {
    return attr_;
  }
  core::optional_t< Attr >
  parse_attribute( core::nothing_t , core::nothing_t , core::nothing_t ) const
  {
    return { attr_ };
  }
  bool parse( core::nothing_t , core::nothing_t , core::nothing_t ) const
  {
    return true;
  }
};

}}}

namespace ep {

template < typename Attr >
rules::primitive::Attribute<Attr> attribute( Attr attr )
{
  return { std::move(attr) };
}

}

namespace ep { namespace core {

template < typename A , typename I >
struct attribute_of< rules::primitive::Attribute<A> , I >
{
  using type = A;
};

}}
