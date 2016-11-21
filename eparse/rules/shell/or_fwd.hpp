#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename A , typename B >
class Or;

}}}

namespace ep {

template < typename A >
constexpr std::decay_t<A>
or_( A&& a );
template < typename A , typename B >
constexpr rules::shell::Or< std::decay_t<A> , std::decay_t<B> >
or_( A&& a , B&& b );
template < typename A , typename B , typename C , typename ... Ds >
constexpr auto
or_( A&& a , B&& b , C&& c , Ds&& ... ds );

}
