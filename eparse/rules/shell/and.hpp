#pragma once

#include "and_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/attribute_of_fwd.hpp"
#include "../../support/tuple_merge.hpp"

#include <utility>

namespace ep { namespace rules { namespace shell {

template < typename A , typename B >
class And
  : public core::expression< And<A,B> >
{
  using this_type = And<A,B>;
protected:
  A a_;
  B b_;

public:
  constexpr And( A a , B b )
    : a_( std::move(a) ) ,
      b_( std::move(b) )
  {
  }

  A& parserA()
  {
    return a_;
  }
  A const& parserA() const
  {
    return a_;
  }
  B& parserB()
  {
    return b_;
  }
  B const& parserB() const
  {
    return b_;
  }

  template < typename I , typename S >
  core::optional_t< typename core::attribute_of<this_type,I>::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    if( auto b = parserB().parse_attribute( begin_ , end , skipper ) )
    {
      if( auto a = parserA().parse_attribute( begin , end , skipper ) )
      {
        return support::tuple_merge_t()( *a , *b );
      }
    }
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    if( parserB().parse( begin_ , end , skipper ) )
    {
      if( parserA().parse( begin , end , skipper ) )
      {
        return true;
      }
    }
    return false;
  }

};

}}}

namespace ep {

template < typename A >
constexpr A
and_( A a )
{
  return a;
}
template < typename A , typename B >
constexpr rules::shell::And<A,B>
and_( A a , B b )
{
  return { std::move(a) ,
           std::move(b) };
}
template < typename A , typename B , typename C , typename ... Ds >
constexpr auto
and_( A a , B b , C c , Ds ... ds )
{
  return and_( 
      and_( std::move(a) , std::move(b) ) ,
      std::move(c) ,
      std::move(ds)...
  );
}

}

namespace ep { namespace core {

template < typename A , typename B , typename I >
struct attribute_of< rules::shell::And<A,B> , I >
{
  using childA = typename attribute_of<A,I>::type;
  using childB = typename attribute_of<B,I>::type;
  using type = typename std::result_of<
    support::tuple_merge_t( childA& , childB& )
  >::type;
};

}}
