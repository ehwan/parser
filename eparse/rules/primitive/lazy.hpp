#pragma once

#include "../../core/expression.hpp"
#include "../../core/optional.hpp"
#include "../../traits/attribute_of_fwd.hpp"

#include <type_traits>
#include <tuple>
#include <utility>

namespace ep { namespace rules { namespace primitive {

template < typename Functor >
class Lazy
  : public core::expression< Lazy<Functor> >
{
  protected:
    Functor functor_;
public:
  
  constexpr Lazy( Functor functor )
    : functor_( std::move(functor) )
  {
  }

  Functor& functor()
  {
    return functor_;
  }
  Functor const& functor() const
  {
    return functor_;
  }
  constexpr decltype(auto) parser()
  {
    return functor_();
  }
  constexpr decltype(auto) parser() const
  {
    return functor_();
  }
  constexpr decltype(auto) operator ()()
  {
    return functor_();
  }
  constexpr decltype(auto) operator ()() const
  {
    return functor_();
  }

  template < typename I , typename S >
  core::optional_t< typename traits::attribute_of<
    std::decay_t< typename std::result_of< Functor() >::type > , I
  >::type >
  parse_attribute( I& begin , I const& end , S const& skipper ) const
  {
    return parser().parse_attribute( begin , end , skipper );
  }
  template < typename I , typename S >
  bool parse( I& begin , I const& end , S const& skipper ) const
  {
    return parser().parse( begin , end , skipper );
  }

  template < typename ... Ts >
  class LazyOperator
  {
  protected:
    Functor functor_;
    std::tuple< Ts... > data_;
  public:
    template < typename F , typename ... Ts_ >
    constexpr LazyOperator( F&& f , Ts_&& ... args )
      : functor_( static_cast< F&& >( f ) ) ,
        data_( static_cast< Ts_&& >( args )... )
    {
    }

    template < size_t ... Ids >
    constexpr decltype(auto) tuple_unpack( std::index_sequence< Ids... > ) const
    {
      return functor_()( std::get<Ids>( data_ )... );
    }
    constexpr decltype(auto) operator ()() const
    {
      return tuple_unapck( std::make_index_sequence< sizeof...( Ts ) >() );
    }
  };

  template < typename ... Ts >
  constexpr auto
  operator()( Ts&& ... args ) const&;

  template < typename ... Ts >
  constexpr auto
  operator()( Ts&& ... args ) &&;

};

}}}

namespace ep {

template < typename Functor >
constexpr rules::primitive::Lazy< Functor >
lazy( Functor functor )
{
  return { std::move(functor) };
}

}

namespace ep { namespace rules { namespace primitive {

template < typename F >
template < typename ... Ts >
constexpr auto
Lazy<F>::operator()( Ts&& ... args ) const&
{
  return lazy( Lazy<F>::LazyOperator< std::decay_t<Ts>... >{ functor_ , static_cast< Ts&& >( args )... } );
}
template < typename F >
template < typename ... Ts >
constexpr auto
Lazy<F>::operator()( Ts&& ... args ) &&
{
  return lazy( Lazy<F>::LazyOperator< std::decay_t<Ts>... >{ std::move( functor_ ) , static_cast< Ts&& >( args )... } );
}

}}}

namespace ep { namespace traits {

template < typename Functor , typename I >
struct attribute_of< rules::primitive::Lazy<Functor> , I >
{
  using type = typename attribute_of< 
    typename std::decay_t< typename std::result_of< Functor() >::type > , I
  >::type;
};

}}
