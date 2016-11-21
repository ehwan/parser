#pragma once

#include "eparse/core/expression.hpp"
#include "eparse/core/optional.hpp"
#include "eparse/core/unused_type.hpp"
#include "eparse/core/variant.hpp"

#include "eparse/traits/attribute_of.hpp"

#include "eparse/rules/primitive/any.hpp"
#include "eparse/rules/primitive/always.hpp"
#include "eparse/rules/primitive/attribute.hpp"
#include "eparse/rules/primitive/eoi.hpp"
#include "eparse/rules/primitive/range.hpp"
#include "eparse/rules/primitive/binary.hpp"
#include "eparse/rules/primitive/one.hpp"
#include "eparse/rules/primitive/string.hpp"
#include "eparse/rules/primitive/lazy.hpp"
#include "eparse/rules/primitive/string_tree.hpp"

#include "eparse/rules/shell/action.hpp"
#include "eparse/rules/shell/repeat.hpp"
#include "eparse/rules/shell/separator.hpp"
#include "eparse/rules/shell/except.hpp"
#include "eparse/rules/shell/optional.hpp"
#include "eparse/rules/shell/capture.hpp"
#include "eparse/rules/shell/sequence.hpp"
#include "eparse/rules/shell/or.hpp"
#include "eparse/rules/shell/skipper.hpp"
#include "eparse/rules/shell/and.hpp"
#include "eparse/rules/shell/ref.hpp"
#include "eparse/rules/shell/any.hpp"
#include "eparse/rules/shell/until.hpp"
#include "eparse/rules/shell/unused.hpp"
#include "eparse/rules/shell/if.hpp"
#include "eparse/rules/shell/debug.hpp"
#include "eparse/rules/shell/bad.hpp"

#include "eparse/rules/define.hpp"

#include "eparse/rules/advanced/unsigned_integer.hpp"
#include "eparse/rules/advanced/fraction.hpp"
#include "eparse/rules/advanced/signature.hpp"
#include "eparse/rules/advanced/unsigned_floating.hpp"
#include "eparse/rules/advanced/signed.hpp"

namespace ep {

template < typename Iterator , typename Parser , typename Skipper >
core::optional_t< typename traits::attribute_of< Parser , Iterator >::type >
parse_attribute( Iterator& begin , Iterator const& end ,
    Parser const& parser , Skipper const& skipper )
{
  core::skip( begin , end , skipper );
  return parser.parse_attribute( begin , end , skipper );
}
template < typename Iterator , typename Parser , typename Skipper >
core::optional_t< typename traits::attribute_of< Parser , Iterator >::type >
parse_attribute( Iterator const& begin , Iterator const& end ,
    Parser const& parser , Skipper const& skipper )
{
  Iterator begin_ = begin;
  core::skip( begin_ , end , skipper );
  return parser.parse_attribute( begin_ , end , skipper );
}

template < typename Iterator , typename Parser , typename Skipper >
bool parse( Iterator& begin , Iterator const& end ,
    Parser const& parser , Skipper const& skipper )
{
  core::skip( begin , end , skipper );
  return parser.parse( begin , end , skipper );
}
template < typename Iterator , typename Parser , typename Skipper >
bool parse( Iterator const& begin , Iterator const& end ,
    Parser const& parser , Skipper const& skipper )
{
  Iterator begin_ = begin;
  core::skip( begin_ , end , skipper );
  return parser.parse( begin_ , end , skipper );
}

}
