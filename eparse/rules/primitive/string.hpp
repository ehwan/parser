#pragma once

#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/nothing.hpp"
#include "../../core/unused_type.hpp"
#include "../../traits/attribute_of_fwd.hpp"
#include "../../support/string_match.hpp"

#include <string>

namespace ep { namespace rules { namespace primitive {

template < typename CharT >
class String
  : public core::expression< String<CharT> >
{
protected:
  std::basic_string<CharT> string_;

public:
  String( std::basic_string<CharT> string )
    : string_( std::move( string ) )
  {
  }
  
  template < typename I >
  core::optional_t< core::unused_type >
  parse_attribute( I& begin , I const& end , core::nothing_t ) const
  {
    if( parse( begin , end , {} ) )
    {
      return core::unused_type();
    }
    return core::none;
  }
  template < typename I >
  bool parse( I& begin , I const& end , core::nothing_t ) const
  {
    I begin_ = begin;
    if( support::string_match( begin , end , string_.begin() , string_.end() , string_.size() ) )
    {
      return true;
    }
    begin = begin_;
    return false;
  }
};

}}}

namespace ep {

template < typename CharT >
rules::primitive::String<CharT>
string( std::basic_string<CharT> str )
{
  return { std::move( str ) };
}
template < typename CharT >
rules::primitive::String<CharT>
string( CharT const* cstr )
{
  return string( std::basic_string<CharT>{ cstr } );
}

}

namespace ep { namespace traits {

template < typename CharT , typename I >
struct attribute_of< rules::primitive::String<CharT> , I >
{
  using type = core::unused_type;
};

}}
