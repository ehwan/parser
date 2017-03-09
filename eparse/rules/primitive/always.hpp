#pragma once

#include "../../core/expression.hpp"
#include "../../core/unused_type.hpp"
#include "../../core/optional.hpp"
#include "../../core/nothing.hpp"
#include "../../core/attribute_of_fwd.hpp"

namespace ep { namespace rules { namespace primitive {

class Always
  : public core::expression< Always >
{
protected:
  bool ret_;

public:
  constexpr Always( bool ret )
    : ret_( ret )
  {
  }

  core::optional_t< core::unused_type >
  parse_attribute( core::nothing_t , core::nothing_t , core::nothing_t ) const
  {
    if( ret_ )
    {
      return core::unused_type();
    }
    return core::none;
  }
  bool parse( core::nothing_t , core::nothing_t , core::nothing_t ) const
  {
    return ret_;
  }

  void set( bool always )
  {
    ret_ = always;
  }
  constexpr bool get() const
  {
    return ret_;
  }
};

}}}

namespace ep {

constexpr rules::primitive::Always always( bool parse_ret )
{
  return { parse_ret };
}
constexpr rules::primitive::Always true_ = always( true );
constexpr rules::primitive::Always false_ = always( false );

}

namespace ep { namespace core {

template < typename I >
struct attribute_of< rules::primitive::Always , I >
{
  using type = core::unused_type;
};

}}
