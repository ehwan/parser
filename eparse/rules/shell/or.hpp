#pragma once

#include "or_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/attribute_of_fwd.hpp"
#include "../../support/variant_merge.hpp"

#include <utility>

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
  core::optional_t< typename core::attribute_of<this_type,I>::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    using ret_type = typename core::attribute_of<this_type,I>::type;
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
constexpr A
or_( A a )
{
  return { std::move(a) };
}
template < typename A , typename B >
constexpr rules::shell::Or<A,B>
or_( A a , B b )
{
  return { std::move(a) ,
           std::move(b) };
}
template < typename A , typename B , typename C , typename ... Ds >
constexpr auto
or_( A a , B b , C c , Ds ... ds )
{
  return or_(
      or_( std::move(a) , std::move(b) ) ,
      std::move(c) , std::move(ds)...
  );
}

}

namespace ep { namespace core {

template < typename A , typename B , typename I >
struct attribute_of< rules::shell::Or<A,B> , I >
{
  using type = typename decltype( support::variant_merge_t()( 
    boost::hana::type_c<typename attribute_of<A,I>::type> ,
    boost::hana::type_c<typename attribute_of<B,I>::type>
  ))::type;
};

}}
