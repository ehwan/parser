#pragma once

#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/nothing.hpp"
#include "../../core/unused_type.hpp"
#include "../../traits/attribute_of_fwd.hpp"
#include "../../support/string_match.hpp"

#include <string>

namespace ep { namespace rules { namespace primitive {

template < typename CharT , typename Compare >
class String
  : public core::expression< String<CharT,Compare> >
{
protected:
  std::basic_string<CharT> string_;
  Compare compare_;

public:
  String( std::basic_string<CharT> string , Compare compare )
    : string_( std::move( string ) ) ,
      compare_( std::move(compare) )
  {
  }

  Compare& compare_function()
  {
    return compare_;
  }
  Compare const& compare_function() const
  {
    return compare_;
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
    if( support::string_match( begin , end , string_.begin() , string_.end() , string_.size() , compare_ ) )
    {
      return true;
    }
    begin = begin_;
    return false;
  }
};

}}}

namespace ep {

template < typename CharT , typename Compare >
rules::primitive::String<CharT,Compare>
string( std::basic_string<CharT> str , Compare cmp )
{
  return { std::move( str ) , std::move(cmp) };
}
template < typename CharT , typename Compare >
rules::primitive::String<CharT,Compare>
string( CharT const* cstr , Compare cmp )
{
  return string( std::basic_string<CharT>{ cstr } , std::move(cmp) );
}

template < typename CharT >
auto string( std::basic_string<CharT> str )
{
  return string( std::move(str) , std::equal_to<CharT>() );
}
template < typename CharT >
auto string( CharT const* cstr )
{
  return string( cstr , std::equal_to<CharT>() );
}

}

namespace ep { namespace traits {

template < typename CharT , typename Compare , typename I >
struct attribute_of< rules::primitive::String<CharT,Compare> , I >
{
  using type = core::unused_type;
};

}}
