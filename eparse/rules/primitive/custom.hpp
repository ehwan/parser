#pragma once

#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/nothing.hpp"
#include "../../core/unused_type.hpp"
#include "../../traits/attribute_of_fwd.hpp"

#include <functional>

namespace ep { namespace rules { namespace primitive {

template < typename Compare >
class Custom
  : public core::expression< Custom<Compare> >
{
protected:
  Compare compare_;
public:
  constexpr Custom( Compare c )
    : compare_( std::move(c) )
  {
  }

  template < typename I >
  core::optional_t< typename traits::attribute_of<Custom<Compare> , I>::type >
  parse_attribute( I& begin , I const& end , core::nothing_t ) const
  {
    if( begin != end )
    {
      if( auto ret = compare_( *begin ) )
      {
        ++begin;
        return ret;
      }
    }
    return core::none;
  }
  template < typename I >
  bool parse( I& begin , I const& end , core::nothing_t ) const
  {
    if( begin != end )
    {
      if( compare_( *begin ) )
      {
        ++begin;
        return true;
      }
    }
    return false;
  }
};

}}}

namespace ep {

template < typename Compare >
constexpr rules::primitive::Custom<Compare>
custom( Compare compare )
{
  return { std::move(compare) };
}

}

namespace ep { namespace traits {

template < typename Compare , typename I >
struct attribute_of< rules::primitive::Custom<Compare> , I >
{
  using type = std::result_of_t<Compare( typename std::iterator_traits<I>::reference )>;
};

}}
