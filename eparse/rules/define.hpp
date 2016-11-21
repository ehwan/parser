#pragma once

#include "primitive/range.hpp"
#include "primitive/one.hpp"
#include "primitive/any.hpp"
#include "shell/or.hpp"
#include "shell//action.hpp"
#include "shell/sequence.hpp"
#include "shell/capture.hpp"
#include "shell/except.hpp"

namespace ep {

constexpr auto alpha = or_( lower , upper );

constexpr auto alnum = or_( digit , alpha );
constexpr auto space = or_(
  tab ,
  whitespace ,
  space20
);
constexpr auto blank = or_(
  tab ,
  space20
);

namespace support {
  struct add_t
  {
    unsigned int data_;
    constexpr add_t( unsigned int data )
      : data_( data )
    {
    }
    template < typename T >
    T operator()( T x ) const
    {
      return x + data_;
    }
  };
  template < unsigned int Max ,
           typename = std::integral_constant<bool , (Max>10)> >
  struct digit_select_t;

  template < unsigned int Max >
  struct digit_select_t< Max , std::true_type >
  {
    constexpr static auto value = or_( 
        digit ,
        action( alpha(Max-10) , add_t(10) )
    );
  };
  template < unsigned int Max >
  struct digit_select_t< Max , std::false_type >
  {
    constexpr static auto value = digit( Max );
  };
}

template < unsigned int Max >
constexpr auto digit_ = support::digit_select_t< Max >::value;

template < typename Start , typename End >
constexpr auto
quoted( Start&& start , End&& end )
{
  return sequence<false>(
      static_cast<Start&&>(start) ,
      capture( star( except( any , static_cast<End&&>(end) ) ) ) ,
      static_cast<End&&>(end)
  );
}

/// parse c-style variable name
constexpr auto var_name = capture( sequence<false>(
  or_( alpha , under_score ) ,
  star( or_( alnum , under_score ) )
) );

}
