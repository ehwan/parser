#pragma once

#include "../core/unused_type.hpp"

#include <tuple>
#include <utility>
#include <type_traits>

namespace ep { namespace support {

struct tuple_merge_t
{
  template < typename A , typename B , size_t ... IdsB >
  decltype(auto)
  push_front( A& a , B& b , std::index_sequence< IdsB... > ) const
  {
    return std::make_tuple(
      std::move(a) ,
      std::get<IdsB>( std::move(b) )...
    );
  }
  template < typename A , typename B , size_t ... IdsA >
  decltype(auto)
  push_back( A& a , B& b , std::index_sequence< IdsA... > ) const
  {
    return std::make_tuple(
      std::get<IdsA>( std::move(a) )... ,
      std::move(b)
    );
  }

  template < typename A , typename B >
  decltype(auto)
  make_tuple( A& a , B& b ) const
  {
    return std::make_tuple( std::move(a) , std::move(b) );
  }
  template < typename A , typename ... Bs >
  decltype(auto)
  make_tuple( A& a , std::tuple< Bs... >& bs )
  {
    return push_front(  a ,
      bs , std::make_index_sequence< sizeof...(Bs) >() );
  }
  template < typename ... As , typename B >
  decltype(auto)
  make_tuple( std::tuple< As... >& a , B& b ) const
  {
    return push_back( a , b ,
        std::make_index_sequence< sizeof...(As) >() );
  }
  template < typename ... As , typename ... Bs >
  decltype(auto)
  make_tuple( std::tuple< As... >& a , std::tuple< Bs... >& b ) const
  {
    return std::tuple_cat( std::move( a ) , std::move( b ) );
  }



  template < typename A , typename B >
  decltype(auto) unused_filter( A& a , B& b ) const
  {
    return make_tuple( a , b );
  }
  core::unused_type unused_filter( core::unused_type , core::unused_type ) const
  {
    return core::unused_type();
  }
  template < typename A >
  A unused_filter( A& a , core::unused_type ) const
  {
    return std::move( a );
  }
  template < typename B >
  B unused_filter( core::unused_type , B& b ) const
  {
    return std::move( b );
  }

  template < typename A , typename B >
  decltype(auto) operator ()( A& a , B& b ) const
  {
    return unused_filter( a , b );
  }

};

}}
