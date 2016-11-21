#pragma once

#include "../core/unused_type.hpp"
#include "../traits/attribute_of_fwd.hpp"
#include <tuple>
#include <utility>
#include <type_traits>

namespace ep { namespace support {

struct action_invoke_t
{
  template < typename F , typename ... Ts , size_t ... Ids >
  auto invoke_tuple( F const& f , std::tuple< Ts... >& tup ,
      std::index_sequence< Ids... > ) const
    -> decltype( f( std::get<Ids>( tup )... ) )
  {
    return f( std::get< Ids >( tup )... );
  }

  template < typename F , typename A >
  auto tuple_filter( F const& f , A& a ) const
    -> decltype( f( a ) )
  {
    return f( a );
  }
  template < typename F , typename ... Ts >
  auto tuple_filter( F const& f , std::tuple< Ts... >& a ) const
    -> decltype( invoke_tuple( f , a , std::make_index_sequence<sizeof...(Ts)>() ) )
  {
    return invoke_tuple( f , a , std::make_index_sequence<sizeof...(Ts)>() );
  }

  template < typename F , typename A , typename B >
  auto pair_filter( F const& f , std::pair<A,B>& a ) const
    -> decltype( f( a.first , a.second ) )
  {
    return f( a.first , a.second );
  }
  template < typename F , typename A >
  auto pair_filter( F const& f , A& a ) const
    -> decltype( tuple_filter( f , a ) )
  {
    return tuple_filter( f , a );
  }

  template < typename F >
  auto unused_filter( F const& f , core::unused_type ) const
    -> decltype( f() )
  {
    return f();
  }
  template < typename F , typename A >
  auto unused_filter( F const& f , A& a ) const
    -> decltype( pair_filter( f , a ) )
  {
    return pair_filter( f , a );
  }
  template < typename F , typename A >
  auto operator ()( F const& f , A& a ) const
    -> decltype( unused_filter( f , a ) )
  {
    return unused_filter( f , a );
  }
};

struct action_void_filter_t
{
  template < typename F , typename A >
  core::unused_type operator ()( F const& f , A& a ,
      typename std::enable_if<
        std::is_void< decltype( action_invoke_t()( f , a ) ) >::value 
      >::type* = nullptr ) const
  {
    action_invoke_t()( f , a );
    return core::unused_type();
  }
  template < typename F , typename A >
  auto operator ()( F const& f , A& a ,
      typename std::enable_if<
        std::is_void< decltype( action_invoke_t()( f , a ) ) >::value==false 
      >::type* = nullptr ) const
    -> decltype( action_invoke_t()( f , a ) )
  {
    return action_invoke_t()( f , a );
  }
};

struct action_unused_parse_t
{
  template < typename I , typename S , typename P , typename F >
  bool operator ()( I& begin , I const& end , S const& skipper ,
      P const& parser , F const& functor ,
      typename std::enable_if<
        std::is_same< core::unused_type , typename traits::attribute_of<P,I>::type >::value
      >::type* = nullptr ) const
  {
    if( parser.parse( begin , end , skipper ) )
    {
      functor();
      return true;
    }
    return false;
  }
  template < typename I , typename S , typename P , typename F >
  bool operator ()( I& begin , I const& end , S const& skipper ,
      P const& parser , F const& functor ,
      typename std::enable_if<
        std::is_same< core::unused_type , typename traits::attribute_of<P,I>::type >::value == false
      >::type* = nullptr ,
      decltype( functor() )* = nullptr ) const
  {
    if( parser.parse( begin , end , skipper ) )
    {
      functor();
      return true;
    }
    return false;
  }
  template < typename I , typename S , typename P , typename F >
  bool operator ()( I& begin , I const& end , S const& skipper ,
      P const& parser , F const& functor ,
      typename std::enable_if<
        std::is_same< core::unused_type , typename traits::attribute_of<P,I>::type >::value == false
      >::type* = nullptr ,
      decltype( 
        action_invoke_t()( functor , *parser.parse_attribute( begin , end , skipper ) )
      )* = nullptr ) const
  {
    if( auto i = parser.parse_attribute( begin , end , skipper ) )
    {
      action_invoke_t()( functor , *i );
      return true;
    }
    return false;
  }
};

}}
