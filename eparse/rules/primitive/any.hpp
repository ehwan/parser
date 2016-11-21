#pragma once

#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/nothing.hpp"
#include "../../traits/attribute_of_fwd.hpp"

#include <iterator>

namespace ep { namespace rules { namespace  primitive {

class Any
  : public core::expression< Any >
{
public:
  
  template < typename I >
  core::optional_t< typename std::iterator_traits<I>::value_type >
  parse_attribute( I& begin , I const& end , core::nothing_t ) const
  {
    if( begin != end )
    {
      return *( begin++ );
    }
    return core::none;
  }
  template < typename I >
  bool parse( I& begin , I const& end , core::nothing_t ) const
  {
    if( begin != end )
    {
      ++begin;
      return true;
    }
    return false;
  }
};

}}}

namespace ep {

constexpr rules::primitive::Any any{};

}

namespace ep { namespace traits {

template < typename I >
struct attribute_of< rules::primitive::Any , I >
{
  using type = typename std::iterator_traits<I>::value_type;
};

}}
