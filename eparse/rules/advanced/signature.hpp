#pragma once

#include "signature_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../traits/attribute_of_fwd.hpp"
#include "../primitive/one.hpp"

#include <type_traits>
#include <utility>

namespace ep { namespace rules { namespace advanced {

template < typename T , typename F >
class Signature
  : public core::expression< Signature<T,F> >
{
protected:
  T true_;
  F false_;
public:
  constexpr Signature( T t , F f )
    : true_( std::move(t) ) ,
      false_( std::move(f) )
  {
  }

  T& trueParser()
  {
    return true_;
  }
  T const& trueParser() const
  {
    return true_;
  }
  F& falseParser()
  {
    return false_;
  }
  F const& falseParser() const
  {
    return false_;
  }

  template < typename I , typename S >
  core::optional_t< bool >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    if( true_.parse( begin , end , skipper ) ){ return true; }
    if( false_.parse( begin , end , skipper ) ){ return false; }
    return true;
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    if( true_.parse( begin , end , skipper ) ){}
    else if( false_.parse( begin , end , skipper ) ){}
    return true;
  }

  template < typename T_ , typename F_ >
  constexpr Signature< std::decay_t<T_> , std::decay_t<F_> >
  operator ()( T_&& true_parser , F_&& false_parser ) const;
  
};

template < typename T , typename F >
constexpr Signature< std::decay_t<T> , std::decay_t<F> >
signature( T&& t , F&& f )
{
  return { static_cast< T&& >( t ) ,
           static_cast< F&& >( f ) };
}

}}}

namespace ep {

constexpr auto signature = rules::advanced::signature( one('+') , one('-') );

}

namespace ep { namespace rules { namespace advanced {


template < typename T_ , typename F_ >
template < typename T , typename F >
constexpr rules::advanced::Signature< std::decay_t<T> , std::decay_t<F> >
Signature<T_,F_>::operator()( T&& true_parser , F&& false_parser ) const
{
  return signature( static_cast<T&&>( true_parser ) ,
                    static_cast<F&&>( false_parser ) );
}

}}}

namespace ep { namespace traits {

template < typename T , typename F , typename I >
struct attribute_of< rules::advanced::Signature<T,F> , I >
{
  using type = bool;
};

}}
