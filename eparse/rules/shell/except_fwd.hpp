#pragma once

namespace ep { namespace rules { namespace shell {

template < typename P , typename E >
class Except;

}}}

namespace ep {

template < typename P , typename E >
constexpr rules::shell::Except<P,E>
except( P parser , E except );

}
