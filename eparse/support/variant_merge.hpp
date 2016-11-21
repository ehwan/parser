#pragma once

#include <boost/hana/type.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/find.hpp>
#include <boost/hana/contains.hpp>
#include <boost/hana/if.hpp>
#include <boost/hana/fold_left.hpp>
#include <boost/hana/append.hpp>
#include <boost/hana/prepend.hpp>
#include <boost/hana/unpack.hpp>

#include "../core/unused_type.hpp"
#include "../core/optional.hpp"
#include "../core/variant_fwd.hpp"

namespace ep { namespace support {

struct variant_merge_t
{
  template < typename ... Ts >
  static boost::hana::tuple< boost::hana::type<Ts>... >
  variant_to_tuple( boost::hana::basic_type< core::variant<Ts...> > )
  {
    return {};
  };
  template < typename T >
  static boost::hana::type<T>
  variant_to_tuple( boost::hana::basic_type< T > )
  {
    return {};
  };
  template < typename ... Ts >
  static auto
  tuple_to_variant( boost::hana::tuple< Ts... > tup )
  {
    return boost::hana::unpack( tup , boost::hana::template_<core::variant> );
  }

  struct variant_filter_t
  {
    template < typename A , typename B >
    boost::hana::tuple< boost::hana::type<A> , boost::hana::type<B> >
    operator ()( boost::hana::basic_type<A> , boost::hana::basic_type<B> ) const
    {
      return {};
    }
    template < typename ... As , typename B >
    auto
    operator ()( boost::hana::tuple< As... > a , boost::hana::basic_type<B> b ) const
    {
      return boost::hana::if_(
        boost::hana::contains( a , b ) ,
        a ,
        boost::hana::append( a , b )
      );
    }
    template < typename A , typename ... Bs >
    auto
    operator ()( boost::hana::basic_type<A> a , boost::hana::tuple< Bs... > b ) const
    {
      return boost::hana::if_(
        boost::hana::contains( b , a ) ,
        b ,
        boost::hana::prepend( b , a )
      );
    }
    template < typename ... As , typename ... Bs >
    auto
    operator ()( boost::hana::tuple< As... > a , boost::hana::tuple< Bs... > b ) const
    {
      return boost::hana::fold_left( b ,
          a , variant_filter_t() );
    }

  };

  template < typename A , typename B >
  auto
  sametype_filter( boost::hana::basic_type<A> a , boost::hana::basic_type<B> b ) const
  {
    return tuple_to_variant(
        variant_filter_t()( variant_to_tuple(a) , variant_to_tuple(b) )
    );
  }
  template < typename A >
  boost::hana::type<A>
  sametype_filter( boost::hana::basic_type<A> , boost::hana::basic_type<A> ) const
  {
    return {};
  }

  template < typename A >
  boost::hana::type<A>
  optional_filter( boost::hana::basic_type< core::optional_t<A> > ) const
  {
    return {};
  }
  template < typename A >
  boost::hana::type<A>
  optional_filter( boost::hana::basic_type<A> ) const
  {
    return {};
  }

  template < typename A , typename B >
  auto
  optional_filter( boost::hana::basic_type<A> a , boost::hana::basic_type<B> b ) const
  {
    return sametype_filter( a , b );
  }
  template < typename A , typename B >
  auto
  optional_filter( boost::hana::basic_type< core::optional_t<A> > a , boost::hana::basic_type<B> b ) const
  {
    return optional_filter( sametype_filter( boost::hana::type_c<A> , b ) );
  }
  template < typename A , typename B >
  auto
  optional_filter( boost::hana::basic_type<A> a , boost::hana::basic_type< core::optional_t<B> > b ) const
  {
    return optional_filter( sametype_filter( a , boost::hana::type_c<B> ) );
  }
  template < typename A , typename B >
  auto
  optional_filter( boost::hana::basic_type< core::optional_t<A> > a , boost::hana::basic_type< core::optional_t<B> > b ) const
  {
    return optional_filter( sametype_filter( boost::hana::type_c<A> , boost::hana::type_c<B> ) );
  }


  boost::hana::type<core::unused_type>
  unused_filter( boost::hana::basic_type<core::unused_type> , boost::hana::basic_type<core::unused_type> ) const
  {
    return {};
  }
  template < typename A >
  auto
  unused_filter( boost::hana::basic_type<A> a , boost::hana::basic_type<core::unused_type> ) const
  {
    return optional_filter( a );
  }
  template < typename B >
  auto
  unused_filter( boost::hana::basic_type<core::unused_type> , boost::hana::basic_type<B> b ) const
  {
    return optional_filter( b );
  }
  template < typename A , typename B >
  auto
  unused_filter( boost::hana::basic_type<A> a , boost::hana::basic_type<B> b ) const
  {
    return optional_filter( a , b );
  }

  template < typename A , typename B >
  auto
  operator ()( boost::hana::basic_type<A> a , boost::hana::basic_type<B> b ) const
  {
    return unused_filter( a , b );
  }
};

}}
