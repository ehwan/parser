#pragma once

#include "sequence_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/skip.hpp"
#include "../../traits/attribute_of_fwd.hpp"
#include "../../support/tuple_merge.hpp"

#include <utility>


namespace ep { namespace rules { namespace shell {

template < typename A , typename B , bool Skip >
class Sequence
  : public core::expression< Sequence<A,B,Skip> >
{
  using this_type = Sequence< A , B , Skip >;
protected:
  A a_;
  B b_;

public:
  constexpr Sequence( A a , B b )
    : a_( std::move(a) ) ,
      b_( std::move(b) )
  {
  }

  A& parserA()
  {
    return a_;
  }
  B& parserB()
  {
    return b_;
  }
  A const& parserA() const
  {
    return a_;
  }
  B const& parserB() const
  {
    return b_;
  }

  template < typename I , typename S , bool Sfinae = Skip >
  void skip( I& begin , I const& end , S const& skipper ,
      typename std::enable_if< Sfinae >::type* = nullptr ) const
  {
    core::skip( begin , end , skipper );
  }
  template < typename I , typename S , bool Sfinae = Skip >
  void skip( I& begin , I const& end , S const& skipper ,
      typename std::enable_if< Sfinae==false >::type* = nullptr ) const
  {
  }

  template < typename I , typename S >
  core::optional_t< typename traits::attribute_of<this_type,I>::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    if( auto a = parserA().parse_attribute( begin , end , skipper ) )
    {
      skip( begin , end , skipper );
      if( auto b = parserB().parse_attribute( begin , end , skipper ) )
      {
        return support::tuple_merge_t()( *a , *b );
      }
    }
    begin = begin_;
    return core::none;
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    I begin_ = begin;
    if( parserA().parse( begin , end , skipper ) )
    {
      skip( begin , end , skipper );
      if( parserB().parse( begin , end , skipper ) )
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

template < bool Skip , typename A >
constexpr std::decay_t<A>
sequence( A&& a )
{
  return static_cast< A&& >( a );
}
template < bool Skip , typename A , typename B >
constexpr rules::shell::Sequence< std::decay_t<A> , std::decay_t<B> , Skip >
sequence( A&& a , B&& b )
{
  return { static_cast< A&& >( a ) ,
           static_cast< B&& >( b ) };
}
template < bool Skip , typename A , typename B , typename C , typename ... Ds >
constexpr auto
sequence( A&& a , B&& b , C&& c , Ds&& ... ds )
{
  return sequence<Skip>( 
      sequence<Skip>( static_cast< A&& >( a ) , static_cast< B&& >( b ) ) ,
      static_cast< C&& >( c ) ,
      static_cast< Ds&& >( ds )...
  );
}

}

namespace ep { namespace traits {

template < typename A , typename B , bool Skip , typename I >
struct attribute_of< rules::shell::Sequence<A,B,Skip> , I >
{
  using childA = typename attribute_of<A,I>::type;
  using childB = typename attribute_of<B,I>::type;
  using type = typename std::result_of<
    support::tuple_merge_t( childA& , childB& )
  >::type;
};

}}
