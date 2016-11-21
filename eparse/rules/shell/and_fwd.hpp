#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename A , typename B >
class And;

}}}

namespace ep {

template < typename A >
constexpr std::decay_t<A>
and_( A&& a );
template < typename A , typename B >
constexpr rules::shell::And< std::decay_t<A> , std::decay_t<B> >
and_( A&& a , B&& b );
template < typename A , typename B , typename C , typename ... Ds >
constexpr auto
and_( A&& a , B&& b , C&& c , Ds&& ... ds );

}
