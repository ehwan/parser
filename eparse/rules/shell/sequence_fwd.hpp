#pragma once

namespace ep { namespace rules { namespace shell {

template < typename A , typename B , bool Skip >
class Sequence;

}}}

namespace ep {

template < bool Skip=true , typename A >
constexpr A
sequence( A a );
template < bool Skip=true , typename A , typename B >
constexpr rules::shell::Sequence<A,B,Skip>
sequence( A a , B b );
template < bool Skip=true , typename A , typename B , typename C , typename ... Ds >
constexpr auto
sequence( A a , B b , C c , Ds ... ds );

}
