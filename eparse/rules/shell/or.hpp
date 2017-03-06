#pragma once
#include "or_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../traits/attribute_of_fwd.hpp"
#include "../../support/variant_merge.hpp"

namespace ep { namespace rules { namespace shell {

template < typename A , typename B >
class Or
  : public core::expression< Or<A,B> >
{
  using this_type = Or<A,B>;

protected:
  A a_;
  B b_;

public:
  constexpr Or( A a , B b )
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
  core::optional_t< typename traits::attribute_of<this_type,I>::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    using ret_type = typename traits::attribute_of<this_type,I>::type;
    if( auto i = parserA().parse_attribute( begin , end , skipper ) )
    {
      return ret_type( *i );
    }
    if( auto i = parserB().parse_attribute( begin , end , skipper ) )
    {
      return ret_type( *i );
    }
    return core::none;
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    if( parserA().parse( begin , end , skipper ) )
    {
      return true;
    }
    if( parserB().parse( begin , end , skipper ) )
    {
      return true;
    }
    return false;
  }
};

}}}

namespace ep {

template < typename A >
constexpr std::decay_t<A>
or_( A&& a )
{
  return { static_cast< A&& >( a ) };
}
template < typename A , typename B >
constexpr rules::shell::Or< std::decay_t<A> , std::decay_t<B> >
or_( A&& a , B&& b )
{
  return { static_cast< A&& >( a ) ,
           static_cast< B&& >( b ) };
}
template < typename A , typename B , typename C , typename ... Ds >
constexpr auto
or_( A&& a , B&& b , C&& c , Ds&& ... ds )
{
  return or_(
      or_( static_cast< A&& >( a ) , static_cast< B&& >( b ) ) ,
      static_cast< C&& >( c ) , static_cast< Ds&& >( ds )...
  );
}

}

namespace ep { namespace traits {

template < typename A , typename B , typename I >
struct attribute_of< rules::shell::Or<A,B> , I >
{
  using type = typename decltype( support::variant_merge_t()( 
    boost::hana::type_c<typename attribute_of<A,I>::type> ,
    boost::hana::type_c<typename attribute_of<B,I>::type>
  ))::type;
};

}}
