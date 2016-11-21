#pragma once

#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/nothing.hpp"
#include "../../core/unused_type.hpp"
#include "../../traits/attribute_of_fwd.hpp"

namespace ep { namespace rules { namespace primitive {

template < typename CharT >
class One
  : public core::expression< One<CharT> >
{
protected:
  CharT data_;
public:
  constexpr One( CharT data )
    : data_( data )
  {
  }
  constexpr CharT data() const
  {
    return data_;
  }

  template < typename I >
  core::optional_t< core::unused_type >
  parse_attribute( I& begin , I const& end , core::nothing_t ) const
  {
    if( begin != end )
    {
      if( *begin == data_ )
      {
        ++begin;
        return core::unused_type();
      }
    }
    return core::none;
  }
  template < typename I >
  bool parse( I& begin , I const& end , core::nothing_t ) const
  {
    if( begin != end && *begin == data_ )
    {
      ++begin;
      return true;
    }
    return false;
  }
};

}}}

namespace ep {

template < typename CharT >
constexpr rules::primitive::One<CharT>
one( CharT ch )
{
  return { ch };
}

constexpr auto tab = one( '\t' );
constexpr auto space20 = one( '\x20' );
constexpr auto cr = one( '\r' );
constexpr auto lf = one( '\n' );
constexpr auto eof = one( '\0' );
constexpr auto double_quote = one( '"' );
constexpr auto single_quote = one( '\'' );
constexpr auto under_score = one( '_' );

}

namespace ep { namespace traits {

template < typename CharT , typename I >
struct attribute_of< rules::primitive::One<CharT> , I >
{
  using type = core::unused_type;
};

}}
