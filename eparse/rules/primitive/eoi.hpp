#pragma once

#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/nothing.hpp"
#include "../../core/unused_type.hpp"
#include "../../traits/attribute_of_fwd.hpp"

namespace ep { namespace rules { namespace primitive {

class EOI
  : public core::expression< EOI >
{
public:
  template < typename I >
  core::optional_t< core::unused_type >
  parse_attribute( I& begin , I const& end , core::nothing_t ) const
  {
    if( begin != end )
    {
      return core::none;
    }
    return core::unused_type();
  }
  template < typename I >
  bool parse( I& begin , I const& end , core::nothing_t ) const
  {
    return ( begin != end ) == false;
  }
};

}}}

namespace ep {

constexpr rules::primitive::EOI eoi{};

}

namespace ep { namespace traits {

template < typename I >
struct attribute_of< rules::primitive::EOI , I >
{
  using type = core::unused_type;
};

}}
