#pragma once

#include "unused_type.hpp"
#include "../rules/shell/any_fwd.hpp"

namespace ep { namespace core {

template < typename I , typename Skipper >
void skip( I& begin , I const& end , Skipper const& skipper )
{
  while( skipper.parse( begin , end , unused_type() ) )
  {
  }
}
template < typename I >
void skip( I& begin , I const& end , unused_type )
{
}
template < typename I >
void skip( I& begin , I const& end , rules::shell::AnySkipper<I> const& );

}}
