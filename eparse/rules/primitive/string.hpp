#pragma once

#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/nothing.hpp"
#include "../../core/unused_type.hpp"
#include "../../traits/attribute_of_fwd.hpp"
#include "../../support/string_match.hpp"

#include <string>

namespace ep { namespace rules { namespace primitive {

template < typename CharT , typename CharTraits >
class String
  : public core::expression< String<CharT,CharTraits> >
{
  struct equals_t
  {
    bool operator()( CharT x , CharT y ) const
    {
      return CharTraits::eq( x , y );
    }
  };
public:
  using string_type = std::basic_string<CharT,CharTraits>;
protected:
  string_type string_;

public:
  String( string_type string )
    : string_( std::move(string) )
  {
  }
  string_type& string()
  {
    return string_;
  }
  string_type const& string() const
  {
    return string_;
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
    if( support::string_match( begin , end , string_.begin() , string_.end() , string_.size() , equals_t() ) )
    {
      return true;
    }
    begin = begin_;
    return false;
  }
};

}}}

namespace ep {

template < typename CharT , typename CharTraits=std::char_traits<CharT> >
rules::primitive::String<CharT,CharTraits>
string( std::basic_string<CharT,CharTraits> str )
{
  return { std::move( str ) };
}
template < typename CharT , typename CharTraits=std::char_traits<CharT> >
rules::primitive::String<CharT,CharTraits>
string( CharT const* cstr )
{
  return { cstr };
}

}

namespace ep { namespace traits {

template < typename CharT , typename CharTraits , typename I >
struct attribute_of< rules::primitive::String<CharT,CharTraits> , I >
{
  using type = core::unused_type;
};

}}
