#pragma once

#include "unsigned_integer.hpp"
#include "unsigned_floating.hpp"
#include "signature.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../traits/attribute_of_fwd.hpp"

#include <type_traits>
#include <utility>

namespace ep { namespace rules { namespace advanced {

template < typename Signature , typename P >
class SignedObject
  : public core::expression< SignedObject<Signature,P> >
{
protected:
  Signature signature_;
  P parser_;
public:
  
  constexpr SignedObject( Signature signature , P parser )
    : signature_( std::move(signature) ) ,
      parser_( std::move(parser) )
  {
  }

  P& parser()
  {
    return parser_;
  }
  P const& parser() const
  {
    return parser_;
  }
  Signature& signature()
  {
    return signature_;
  }
  Signature const& signature() const
  {
    return signature_;
  }

  template < typename I , typename S >
  core::optional_t< std::make_signed_t< typename traits::attribute_of<P,I>::type > >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    using ret_type = std::make_signed_t< typename traits::attribute_of<P,I>::type >;
    I begin_ = begin;
    if( auto sig = signature().parse_attribute( begin , end , skipper ) )
    {
      if( auto obj = parser().parse_attribute( begin , end , skipper ) )
      {
        const ret_type ret = static_cast< ret_type >( *obj );
        return *sig ? ret : -ret;
      }
    }
    begin = begin_;
    return core::none;
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    if( signature().parse( begin , end , skipper ) )
    {
      if( auto obj = parser().parse( begin , end , skipper ) )
      {
        return true;
      }
    }
    begin = begin_;
    return false;
  }
};

}}}

namespace ep {

template < typename Signature , typename Parser >
constexpr rules::advanced::SignedObject< std::decay_t<Signature> , std::decay_t<Parser> >
signed_numeric( Signature&& signature , Parser&& parser )
{
  return { static_cast< Signature&& >( signature ) ,
           static_cast< Parser&& >( parser ) };
}
template < typename Parser >
constexpr auto
signed_numeric( Parser&& parser )
{
  return signed_numeric( signature , static_cast< Parser&& >( parser ) );
}

constexpr auto int_ = signed_numeric( uint );

constexpr auto float_  = signed_numeric( ufloat );
constexpr auto double_ = signed_numeric( udouble );
constexpr auto ldouble = signed_numeric( uldouble );


}

namespace ep { namespace traits {

template < typename S , typename P , typename I >
struct attribute_of< rules::advanced::SignedObject<S,P> , I >
{
  using type = std::make_signed_t< typename attribute_of<P,I>::type >;
};

}}
