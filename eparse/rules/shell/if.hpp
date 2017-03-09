#pragma once

#include "if_fwd.hpp"
#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../core/attribute_of_fwd.hpp"
#include "../../support/variant_merge.hpp"
#include <tuple>

#include <utility>

namespace ep { namespace rules { namespace shell {

template < typename ... Ts >
class Switch
  : public core::expression< Switch<Ts...> >
{
  using this_type = Switch< Ts... >;
protected:
  unsigned int index_;
  std::tuple< Ts... > data_;

public:
  template < typename ... Ts_ >
  constexpr Switch( unsigned int id , Ts_&&... data )
    : index_( id ) ,
      data_( static_cast< Ts_&& >( data )... )
  {
  }

  template < unsigned int ID , typename I , typename S >
  core::optional_t< typename core::attribute_of<this_type,I>::type >
  parseattr( I& begin , I const& end , S const& skipper ,
      unsigned int id ) const
  {
    if( id )
    {
      return parseattr<ID+1>( begin , end , skipper , id-1 );
    }else
    {
      return std::get<ID>( data_ ).parse_attribute( begin , end , skipper );
    }
  }
  template < unsigned int ID , typename I , typename S >
  bool parsenoattr( I& begin , I const& end , S const& skipper ,
      unsigned int id ) const
  {
    if( id )
    {
      return parsenoattr<ID+1>( begin , end , skipper , id-1 );
    }else
    {
      return std::get<ID>( data_ ).parse( begin , end , skipper );
    }
  }

  template < typename I , typename S >
  core::optional_t< typename core::attribute_of<this_type,I>::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    return parseattr<0>( begin , end , skipper , index_ );
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    return parsenoattr<0>( begin , end , skipper , index_ );
  }
      
};

template < typename A , typename B >
class If
  : public core::expression< If<A,B> >
{
  using this_type = If<A,B>;
protected:
  A a_;
  B b_;
  bool is_a_;
public:
  constexpr If( A a , B b , bool is_a )
    : a_( std::move(a) ) ,
      b_( std::move(b) ) ,
      is_a_( is_a )
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
  bool isA() const
  {
    return is_a_;
  }
  void set( bool isA )
  {
    is_a_ = isA;
  }

  template < typename I , typename S >
  core::optional_t< typename core::attribute_of<this_type,I>::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    if( isA() )
    {
      return { parserA().parse_attribute( begin , end , skipper ) };
    }else
    {
      return { parserB().parse_attribute( begin , end , skipper ) };
    }
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    if( isA() )
    {
      return parserA().parse( begin , end , skipper );
    }else
    {
      return parserB().parse( begin , end , skipper );
    }
  }
};

}}}

namespace ep {

template < typename A , typename B >
constexpr rules::shell::If< std::decay_t<A> , std::decay_t<B> >
if_( bool isA , A&& a , B&& b )
{
  return { static_cast< A&& >( a ) ,
           static_cast< B&& >( b ) ,
           isA };
}

template < typename ... Ts >
constexpr rules::shell::Switch< std::decay_t<Ts>... >
switch_( unsigned int index ,
    Ts&&... ps )
{
  return { index , static_cast<Ts&&>(ps)... };
}

}

namespace ep { namespace core {

template < typename A , typename I >
struct attribute_of< rules::shell::Switch<A> , I >
{
  using type = typename attribute_of<A,I>::type;
};
template < typename A , typename B , typename ... Ts , typename I >
struct attribute_of< rules::shell::Switch<A,B,Ts...> , I >
{
  using type2 = typename attribute_of< rules::shell::Switch<B,Ts...> , I >::type;

  using type = typename decltype( support::variant_merge_t()(
      boost::hana::type_c< typename attribute_of<A,I>::type > ,
      boost::hana::type_c< type2 >
  ) )::type;
};
template < typename A , typename B , typename I >
struct attribute_of< rules::shell::If<A,B> , I >
{
  using type = typename decltype( support::variant_merge_t()( 
    boost::hana::type_c<typename attribute_of<A,I>::type> ,
    boost::hana::type_c<typename attribute_of<B,I>::type>
  ))::type;
};

}}
