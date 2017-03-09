#pragma once

namespace ep { namespace rules { namespace shell {

template < typename A , typename B >
class If;

}}}

namespace ep {

template < typename A , typename B >
constexpr rules::shell::If<A,B>
if_( bool isA , A a , B b );

}
