#pragma once

#include <type_traits>

namespace ep { namespace rules { namespace shell {

template < typename A , typename B >
class If;

}}}

namespace ep {

template < typename A , typename B >
constexpr rules::shell::If< std::decay_t<A> , std::decay_t<B> >
if_( bool isA , A&& a , B&& b );

template < typename A >
constexpr std::decay_t<A>
switch_( int index , A&& a );

template < typename A , typename B , typename ... Ts >
constexpr auto
switch_( int index , A&& a , B&& b , Ts&& ... args );

}
